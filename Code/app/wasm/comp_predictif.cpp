#include "dynamic_bitset.hpp"

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <deque>

static void pushBitset(sul::dynamic_bitset<>& bitset, unsigned value, unsigned bits) {
    for (unsigned i{}; i < bits; ++i) {
        bitset.push_back(value & (1 << i));
    }
}

extern "C" {

    std::size_t compressionPredictif(std::uint8_t* inputImage, std::uint8_t* predictedImage, int width, int height, int method) {
        sul::dynamic_bitset<> outputData;
        predictedImage[0] = inputImage[0];
        predictedImage[1] = inputImage[1];
        predictedImage[2] = inputImage[2];
        for (int i{}; i < width; ++i) {
            for (int j{}; j < height; ++j) {
                for (int k{}; k < 3; ++k) {
                    int position{i + j * width};
                    int p{static_cast<int>(inputImage[3 * position + k])};
                    if (i == 0) {
                        if (j == 0) {
                            continue;
                        }
                        int x{inputImage[3 * (position - width) + k]};
                        predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - x + 128, 0, 255));
                    }
                    else if (j == 0) {
                        int x{inputImage[3 * (position - 1) + k]};
                        predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - x + 128, 0, 255));
                    }
                    else {
                        int a{inputImage[3 * (position - width - 1) + k]};
                        int b{inputImage[3 * (position - width) + k]};
                        int c{inputImage[3 * (position - 1) + k]};
                        switch (method) {
                            default:
                            case 0:
                                predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - (b + c) / 2 + 128, 0, 255));
                                break;
                            // DPCM
                            case 1:
                                predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - (b + c - a) + 128, 0, 255));
                                break;
                            // MED
                            case 2:
                                if (a >= std::max(b, c)) {
                                    predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - std::min(b, c) + 128, 0, 255));
                                }
                                else if (a <= std::min(b, c)) {
                                    predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - std::max(b, c) + 128, 0, 255));
                                }
                                else {
                                    predictedImage[3 * position + k] = static_cast<std::uint8_t>(std::clamp(p - (b + c - a) + 128, 0, 255));
                                }
                                break;
                        }                    
                    }
                }
            }
        }

        unsigned maxLength{};
        unsigned length{};
        for (int i{}; i < width * height; ++i) {
            for (int j{}; j < 3; ++j) {
                if (predictedImage[3 * i + j] != 128) {
                    maxLength = std::max(maxLength, length);
                    length = 0;
                    goto breakContinue1;
                }
            }
            ++length;
            breakContinue1:;
        }
        maxLength = std::max(maxLength, length);
        unsigned bitsForLength{static_cast<unsigned>(std::ceil(std::log2(maxLength)))};
        pushBitset(outputData, bitsForLength, 6);

        int latest{};
        length = 0;
        for (int i{1}; i < width * height; ++i) {
            for (int j{}; j < 3; ++j) {
                if (predictedImage[3 * i + j] != 128) {
                    for (int k{}; k < 3; ++k) {
                        pushBitset(outputData, predictedImage[3 * latest + k], 8);
                    }

                    pushBitset(outputData, length, bitsForLength);
                    latest = i;
                    length = 0;
                    goto breakContinue2;
                }
            }
            ++length;
            breakContinue2:;
        }

        if (length != 0) {
            for (int k{}; k < 3; ++k) {
                pushBitset(outputData, predictedImage[3 * latest + k], 8);
            }

            pushBitset(outputData, length, bitsForLength);
        }

        return outputData.size();
    }
}