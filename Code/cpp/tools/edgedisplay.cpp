#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_image_write.h"

#include <iostream>

void displayEdges(std::uint8_t* inputImage, std::uint8_t* segmentatedImage, std::uint8_t* outputImage, int width, int height) {
    for (int i{}; i < width - 1; ++i) {
        for (int j{}; j < height - 1; ++j) {
            int k{i + j * width};
            bool rightEdge{segmentatedImage[3 * k] != segmentatedImage[3 * (k + 1)] || segmentatedImage[3 * k + 1] != segmentatedImage[3 * (k + 1) + 1] || segmentatedImage[3 * k + 2] != segmentatedImage[3 * (k + 1) + 2]};
            bool bottomEdge{segmentatedImage[3 * k] != segmentatedImage[3 * (k + width)] || segmentatedImage[3 * k + 1] != segmentatedImage[3 * (k + width) + 1] || segmentatedImage[3 * k + 2] != segmentatedImage[3 * (k + width) + 2]};
            if (rightEdge || bottomEdge) {
                outputImage[3 * k] = 0;
                outputImage[3 * k + 1] = 0;
                outputImage[3 * k + 2] = 0;
                if (rightEdge) {
                    outputImage[3 * (k + 1)] = 0;
                    outputImage[3 * (k + 1) + 1] = 0;
                    outputImage[3 * (k + 1) + 2] = 0;
                }
                if (bottomEdge) {
                    outputImage[3 * (k + width)] = 0;
                    outputImage[3 * (k + width) + 1] = 0;
                    outputImage[3 * (k + width) + 2] = 0;
                }
            }
            else {
                outputImage[3 * k] = inputImage[3 * k];
                outputImage[3 * k + 1] = inputImage[3 * k + 1];
                outputImage[3 * k + 2] = inputImage[3 * k + 2];
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Séparation des différents superpixels d'une image\nUsage :\n  - Image d'entrée (peut être l'image segmentée)\n  - Image segmentée\n  - Image de sortie\n";
        return 0;
    }

    int width;
    int height;
    int channelsCount;
    std::uint8_t* inputImage{stbi_load(argv[1], &width, &height, &channelsCount, STBI_rgb)};
    std::uint8_t* segmentedImage{stbi_load(argv[2], &width, &height, &channelsCount, STBI_rgb)};
    std::uint8_t* outputImage = new std::uint8_t[3 * width * height];
    
    displayEdges(inputImage, segmentedImage, outputImage, width, height);

    stbi_write_png(argv[3], width, height, STBI_rgb, outputImage, 3 * width);

    delete[] outputImage;
    stbi_image_free(inputImage);
    stbi_image_free(segmentedImage);
}