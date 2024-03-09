// https://geekyrakshit.dev/geekyrakshit-blog/algebra/computervision/convolution/maths/python/2020/09/17/efficient-graph-based-image-segmentation.html#implementation

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <algorithm>
#include <string>

struct Component {
    std::map<Component*, double> minConnectivity;
    std::vector<int> pixels;
    double internalMaxMST;
    double r;
    double g;
    double b;
};

double getPixelWeight(std::uint8_t* inputImage, int width, int height, int x, int y) {
    int pixelPosition{3 * (x + y * width)}; 
    double dR{static_cast<double>(inputImage[pixelPosition])};
    double dG{static_cast<double>(inputImage[pixelPosition + 1])};
    double dB{static_cast<double>(inputImage[pixelPosition + 2])};
    double dX{static_cast<double>(x)};
    double dY{static_cast<double>(x)};
    return std::sqrt(/*dX * dX + dY * dY + */dR * dR + dG * dG + dB * dB);
}

void felzenszwalb(std::uint8_t* inputImage, std::uint8_t* outputImage, int width, int height, double c, int k) {

    // Initialisation
    // Calcul des poids des pixels et création des components (où chaque pixel est son propre component)
    std::vector<double> weights(width * height);
    std::vector<Component*> componentMap(width * height);
    for (int i{}; i < width; ++i) {
        for (int j{}; j < height; ++j) {
            int pixelPosition{i + j * width};
            weights[pixelPosition] = getPixelWeight(inputImage, width, height, i, j);
            componentMap[pixelPosition] = new Component{
                {},
                {pixelPosition},
                0.0,
                static_cast<double>(inputImage[3 * pixelPosition]),
                static_cast<double>(inputImage[3 * pixelPosition + 1]),
                static_cast<double>(inputImage[3 * pixelPosition + 2]),
            };
        }
    }

    // Construction des arêtes horizontales
    int horizontalEdgesCount{(width - 1) * height};
    std::vector<double> horizontalEdges(horizontalEdgesCount);
    int iHorizontal{};
    for (int i{}; i < width - 1; ++i) {
        for (int j{}; j < height; ++j) {
            horizontalEdges[iHorizontal++] = i + j * width;
        }
    }

    // Tri des arêtes horizontales dans l'ordre croissant
    std::sort(horizontalEdges.begin(), horizontalEdges.end(), [&weights](double id1, double id2) {
        return std::abs(weights[id1] - weights[id1 + 1]) < std::abs(weights[id2] - weights[id2 + 1]);
    });

    // Construction des arêtes verticales
    int verticalEdgesCount{width * (height - 1)};
    std::vector<double> verticalEdges(verticalEdgesCount);
    int iVertical{};
    for (int i{}; i < width; ++i) {
        for (int j{}; j < height - 1; ++j) {
            verticalEdges[iVertical++] = i + j * width;
        }
    }

    // Tri des arêtes verticales dans l'ordre croissant
    std::sort(verticalEdges.begin(), verticalEdges.end(), [&weights, width](double id1, double id2) {
        return std::abs(weights[id1] - weights[id1 + width]) < std::abs(weights[id2] - weights[id2 + width]);
    });

    int componentsCount{width * height};

    // Itération
    iHorizontal = 0;
    iVertical = 0;
    for (int i{}; i < horizontalEdgesCount + verticalEdgesCount; ++i) {
        int firstPixel;
        int secondPixel;

        if (iHorizontal < horizontalEdgesCount && (iVertical >= verticalEdgesCount || horizontalEdges[iHorizontal] <= verticalEdges[iVertical])) {
            firstPixel = horizontalEdges[iHorizontal];
            secondPixel = firstPixel + 1;
            ++iHorizontal;
        }
        else {
            firstPixel = verticalEdges[iVertical];
            secondPixel = firstPixel + width;
            ++iVertical;
        }

        Component* firstComponent{componentMap[firstPixel]};
        Component* secondComponent{componentMap[secondPixel]};

        // Les pixels sont déjà dans le même component, on ne fait rien
        if (firstComponent == secondComponent) {
            continue;
        }

        double edgeWeight{std::abs(weights[firstPixel] - weights[secondPixel])};

        // La différence entre deux components est le poids minimal d'une arête qui connecte les deux components
        // Sachant que les arêtes sont parcourues dans l'ordre croissant de leur poids, c'est donc toujours la première
        if (firstComponent->minConnectivity.find(secondComponent) == firstComponent->minConnectivity.end()) {
            firstComponent->minConnectivity[secondComponent] = edgeWeight;
            // On ne stocke que la connectivité des components droite et bas
        }
        
        // Si l'équation satisfait le prédicat de fusion
        std::size_t firstComponentPixelCount{firstComponent->pixels.size()};
        std::size_t secondComponentPixelCount{firstComponent->pixels.size()};
        double tau1{c / firstComponentPixelCount};
        double tau2{c / secondComponentPixelCount};
        if (firstComponent->minConnectivity[secondComponent] <= std::min(firstComponent->internalMaxMST + tau1, secondComponent->internalMaxMST + tau2)) {

            // Le poids de l'arête devient le poids maximal du MST de cette fusion
            firstComponent->internalMaxMST = edgeWeight;

            // Calcul de la nouvelle moyenne des pixels de la fusion
            double denom{1.0 / (firstComponentPixelCount + secondComponentPixelCount)};

            firstComponent->r = denom * (firstComponent->r * firstComponentPixelCount + secondComponent->r * secondComponentPixelCount);
            firstComponent->g = denom * (firstComponent->g * firstComponentPixelCount + secondComponent->g * secondComponentPixelCount);
            firstComponent->b = denom * (firstComponent->b * firstComponentPixelCount + secondComponent->b * secondComponentPixelCount);

            // Fusion des components
            for (int pixel : secondComponent->pixels) {
                componentMap[pixel] = firstComponent;
            }
            firstComponent->pixels.insert(firstComponent->pixels.end(), secondComponent->pixels.begin(), secondComponent->pixels.end());

            // Partage de la connectivité
            firstComponent->minConnectivity.erase(secondComponent);
            for (const auto& pair : secondComponent->minConnectivity) {
                firstComponent->minConnectivity.insert(pair);
            }

            --componentsCount;

            delete secondComponent;

            if (componentsCount <= k) {
                break;
            }
        }
    }

    // Construction de l'image finale
    for (int i{}; i < width * height; ++i) {
        outputImage[3 * i] = static_cast<std::uint8_t>(std::round(componentMap[i]->r));
        outputImage[3 * i + 1] = static_cast<std::uint8_t>(std::round(componentMap[i]->g));
        outputImage[3 * i + 2] = static_cast<std::uint8_t>(std::round(componentMap[i]->b));
    }

}

int main(int argc, const char** argv) {
    if (argc < 5) {
        std::cout << "Usage :\n  - Image d'entrée\n  - Image de sortie\n  - Coefficient de connexivité C (20.0 par exemple)\n  - Nombre maximum de superpixels K\n";
        return 0;
    }

    int width;
    int height;
    int channelsCount;
    std::uint8_t* inputImage{stbi_load(argv[1], &width, &height, &channelsCount, STBI_rgb)};
    std::uint8_t* outputImage = new std::uint8_t[3 * width * height];
    
    felzenszwalb(inputImage, outputImage, width, height, std::atof(argv[3]), std::atoi(argv[4]));

    stbi_write_png(argv[2], width, height, STBI_rgb, outputImage, 3 * width);

    delete[] outputImage;
    stbi_image_free(inputImage);

}