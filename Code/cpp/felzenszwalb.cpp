// https://geekyrakshit.dev/geekyrakshit-blog/algebra/computervision/convolution/maths/python/2020/09/17/efficient-graph-based-image-segmentation.html#implementation

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "colorm.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>
#include <map>
#include <algorithm>
#include <string>

struct Component {
    std::vector<int> pixels;
    double internalMaxMST;
    colorm::Lab color;
};

void felzenszwalb(std::uint8_t* inputImage, std::uint8_t* outputImage, int width, int height, double c) {
    c /= 255.0;

    // Initialisation
    // Calcul des poids des pixels et création des components (où chaque pixel est son propre component)
    std::vector<colorm::Lab> pixelColors(width * height);
    std::vector<Component*> componentMap(width * height);
    for (int i{}; i < width; ++i) {
        for (int j{}; j < height; ++j) {
            int pixelPosition{i + j * width};
            colorm::Lab pixelColor{colorm::Rgb{
                static_cast<double>(inputImage[3 * pixelPosition]),
                static_cast<double>(inputImage[3 * pixelPosition + 1]),
                static_cast<double>(inputImage[3 * pixelPosition + 2])
            }};
            pixelColors[pixelPosition] = pixelColor;
            componentMap[pixelPosition] = new Component{
                {pixelPosition},
                0.0,
                pixelColor
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
    std::sort(horizontalEdges.begin(), horizontalEdges.end(), [&pixelColors](double id1, double id2) {
        return pixelColors[id1].distance(pixelColors[id1 + 1]) < pixelColors[id2].distance(pixelColors[id2 + 1]);
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
    std::sort(verticalEdges.begin(), verticalEdges.end(), [&pixelColors, width](double id1, double id2) {
        return pixelColors[id1].distance(pixelColors[id1 + width]) < pixelColors[id2].distance(pixelColors[id2 + width]);
    });

    int componentsCount{width * height};

    // Itération
    iHorizontal = 0;
    iVertical = 0;
    for (int i{}; i < horizontalEdgesCount + verticalEdgesCount; ++i) {
        int firstPixel;
        int secondPixel;
        bool checkHorizontal{iHorizontal < horizontalEdgesCount};
        bool checkVertical{iVertical < verticalEdgesCount};
        if (checkHorizontal && checkVertical) {
            double edgeWeightHorizontal{pixelColors[horizontalEdges[iHorizontal]].distance(pixelColors[horizontalEdges[iHorizontal] + 1])};
            double edgeWeightVertical{pixelColors[verticalEdges[iVertical]].distance(pixelColors[verticalEdges[iVertical] + width])};
            checkHorizontal = edgeWeightHorizontal <= edgeWeightVertical;
        }

        if (checkHorizontal) {
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

        double edgeWeight{pixelColors[firstPixel].distance(pixelColors[secondPixel])};

        // Si l'équation satisfait le prédicat de fusion
        std::size_t firstComponentPixelCount{firstComponent->pixels.size()};
        std::size_t secondComponentPixelCount{firstComponent->pixels.size()};
        if (edgeWeight <= std::min(firstComponent->internalMaxMST + c / firstComponentPixelCount, secondComponent->internalMaxMST + c / secondComponentPixelCount)) {

            // Le poids de l'arête devient le poids maximal du MST de cette fusion (car il connecte les deux parties du graphe dont les arêtes ont un poids inférieur)
            firstComponent->internalMaxMST = edgeWeight;

            // Calcul de la nouvelle moyenne des pixels de la fusion
            double denom{1.0 / (firstComponentPixelCount + secondComponentPixelCount)};

            firstComponent->color.setLightness(denom * (firstComponent->color.lightness() * firstComponentPixelCount + secondComponent->color.lightness() * secondComponentPixelCount));
            firstComponent->color.setA(denom * (firstComponent->color.a() * firstComponentPixelCount + secondComponent->color.a() * secondComponentPixelCount));
            firstComponent->color.setB(denom * (firstComponent->color.b() * firstComponentPixelCount + secondComponent->color.b() * secondComponentPixelCount));

            // Fusion des components
            for (int pixel : secondComponent->pixels) {
                componentMap[pixel] = firstComponent;
            }
            firstComponent->pixels.insert(firstComponent->pixels.end(), secondComponent->pixels.begin(), secondComponent->pixels.end());

            --componentsCount;

            delete secondComponent;
        }
    }

    // Construction de l'image finale
    for (int i{}; i < width * height; ++i) {
        colorm::Rgb rgbColor{componentMap[i]->color};
        outputImage[3 * i] = static_cast<std::uint8_t>(std::round(rgbColor.red()));
        outputImage[3 * i + 1] = static_cast<std::uint8_t>(std::round(rgbColor.green()));
        outputImage[3 * i + 2] = static_cast<std::uint8_t>(std::round(rgbColor.blue()));
    }

}

int main(int argc, const char** argv) {
    if (argc < 4) {
        std::cout << "Usage :\n  - Image d'entrée\n  - Image de sortie\n  - Seuil k (300.0 par exemple)\n";
        return 0;
    }

    int width;
    int height;
    int channelsCount;
    std::uint8_t* inputImage{stbi_load(argv[1], &width, &height, &channelsCount, STBI_rgb)};
    std::uint8_t* outputImage = new std::uint8_t[3 * width * height];
    
    felzenszwalb(inputImage, outputImage, width, height, std::atof(argv[3]));

    stbi_write_png(argv[2], width, height, STBI_rgb, outputImage, 3 * width);

    delete[] outputImage;
    stbi_image_free(inputImage);
}