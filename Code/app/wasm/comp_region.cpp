#include "dynamic_bitset.hpp"

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <set>
#include <deque>

static void pushBitset(sul::dynamic_bitset<>& bitset, unsigned value, unsigned bits) {
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

extern "C" {

    std::size_t compressionRegion(std::uint8_t* inputImage, std::uint8_t* edgeImage, int width, int height) {
        sul::dynamic_bitset<> outputData;
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

        return outputData.size();
    }

}