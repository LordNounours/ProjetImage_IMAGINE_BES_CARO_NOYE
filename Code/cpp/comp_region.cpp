#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "dynamic_bitset.hpp"

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <set>
#include <deque>

void pushBitset(sul::dynamic_bitset<>& bitset, unsigned value, unsigned bits) {
    for (unsigned i{}; i < bits; ++i) {
        bitset.push_back(value & (1 << i));
    }
}

struct Region {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
    std::vector<int> pixels;
};

void compression(std::uint8_t* inputImage, std::uint8_t* edgeImage, sul::dynamic_bitset<>& outputData, int width, int height) {
    int positionBits{static_cast<int>(std::ceil(std::log(width * height)))};

    std::vector<Region*> pixelRegions(width * height);
    for (int i{}; i < width * height; ++i) {
        pixelRegions[i] = new Region{
            inputImage[3 * i],
            inputImage[3 * i + 1],
            inputImage[3 * i + 2],
            {i}
        };
    }

    for (int i{}; i < width - 1; ++i) {
        for (int j{}; j < height; ++j) {
            int position{i + j * width};
            Region* current{pixelRegions[position]};
            Region* next{pixelRegions[position + 1]};

            if (current->r == next->r && current->g == next->g && current->b == next->b) {
                current->pixels.insert(current->pixels.end(), next->pixels.begin(), next->pixels.end());
                pixelRegions[position + 1] = current;
                delete next;
            }
        }
    }
    
    for (int j{}; j < height - 1; ++j) {
        for (int i{}; i < width; ++i) {
            int position{i + j * width};
            Region* current{pixelRegions[position]};
            Region* next{pixelRegions[position + width]};

            if (current != next && current->r == next->r && current->g == next->g && current->b == next->b) {
                current->pixels.insert(current->pixels.end(), next->pixels.begin(), next->pixels.end());
                for (int pixel : next->pixels) {
                    pixelRegions[pixel] = current;
                }
                delete next;
            }
        }
    }

    sul::dynamic_bitset edgeMap(width * height);
    std::set<Region*> regions;
    for (int i{}; i < width; ++i) {
        for (int j{}; j < height; ++j) {
            int position{i + j * width};
            if (i > 0 && j > 0 && i < width && j < width && pixelRegions[position - 1] == pixelRegions[position] && pixelRegions[position + 1] == pixelRegions[position] && pixelRegions[position - width] == pixelRegions[position] && pixelRegions[position + width] == pixelRegions[position]) {
                edgeMap[position] = false;
                for (int k{}; k < 3; ++k) {
                    edgeImage[3 * position + k] = 0;
                }
            }
            else {
                edgeMap[position] = true;
                for (int k{}; k < 3; ++k) {
                    edgeImage[3 * position + k] = inputImage[3 * position + k];
                }
                regions.insert(pixelRegions[position]);
            }
        }
    }

    pushBitset(outputData, positionBits, 6);
    for (Region* region : regions) {

        std::deque<int> edges;
        for (int pixel : region->pixels) {
            if (edgeMap[pixel]) {
                edges.push_back(pixel);
            }
        }

        pushBitset(outputData, edges.size(), positionBits);
        pushBitset(outputData, region->r, 8);
        pushBitset(outputData, region->g, 8);
        pushBitset(outputData, region->b, 8);
        for (int pixel : edges) {
            pushBitset(outputData, pixel, positionBits);
        }

        delete region;
    }
}

int main(int argc, const char** argv) {
    if (argc < 3) {
        std::cout << "Usage :\n  - Image d'entrée\n  - Image contours\n";
        return 0;
    }

    int width;
    int height;
    int channelsCount;
    std::uint8_t* inputImage{stbi_load(argv[1], &width, &height, &channelsCount, STBI_rgb)};

    std::uint8_t* edgeImage = new std::uint8_t[3 * width * height];
    sul::dynamic_bitset outputData;
    
    compression(inputImage, edgeImage, outputData, width, height);

    std::size_t previousSize{24ull * width * height};
    std::size_t currentSize{outputData.size()};
    std::cout << "Taille (non compressée) : " << previousSize << " bits\n";
    std::cout << "Taille (compressée) : " << currentSize << " bits\n";
    std::cout << "Débit : " << static_cast<double>(currentSize) / (width * height) << " bits/pixel\n";
    std::cout << "Taux de compression : " << static_cast<double>(previousSize) / currentSize << '\n';

    stbi_write_png(argv[2], width, height, STBI_rgb, edgeImage, 3 * width);

    delete[] edgeImage;
    stbi_image_free(inputImage);
}