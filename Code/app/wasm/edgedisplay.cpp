#include <iostream>

extern "C" {

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
}