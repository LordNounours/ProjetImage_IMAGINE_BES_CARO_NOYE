#include <iostream>

#include "colorm.h"

#include <stack>
#include <cstdint>
#include <limits>
#include <vector>
#include <deque>

struct Node {
    int pixel;
    std::vector<Node*> lowerDensityNeighbors;
};

double parzenDensity(colorm::Lab* colorMap, int width, int height, int x, int y, double sigma, int kernelSize) {
    int posPixel{x + y * width};
    double density{};
    double denom{1.0 / (2.0 * sigma * sigma)};
    for (int i{-kernelSize}; i <= kernelSize; ++i) {
        for (int j{-kernelSize}; j <= kernelSize; ++j) {
            int posX{x + i};
            int posY{y + j};
            if (posX >= 0 && posY >= 0 && posX < width && posY < height) {
                int posNeighbor{posX + posY * width};
                int dx{x - posX};
                int dy{y - posY};
                double dL{colorMap[posPixel].lightness() - colorMap[posNeighbor].lightness()};
                double da{colorMap[posPixel].a() - colorMap[posNeighbor].a()};
                double db{colorMap[posPixel].b() - colorMap[posNeighbor].b()};
                double exponent{(dx * dx + dy * dy + dL * dL + da * da + db * db) * denom};
                double a{0.465 * exponent + 1.0};
                double a2{a * a};
                density += 2.0 / (1.0 + a2 * a2); // Approximation de std::exp(-x) pour x >= 0
            }
        }
    }
    return density;
}

int findFittingNeighbor(double* densityMap, colorm::Lab* colorMap, int width, int height, int x, int y, int kernelSize) {
    int selectedPos{-1};
    double minDistance{std::numeric_limits<double>::infinity()};
    for (int i{-kernelSize}; i <= kernelSize; ++i) {
        for (int j{-kernelSize}; j <= kernelSize; ++j) {
            int posX{x + i};
            int posY{y + j};
            if (posX >= 0 && posY >= 0 && posX < width && posY < width) {
                int posPixel{x + y * width};
                int posNeighbor{posX + posY * width};
                int dx{x - posX};
                int dy{y - posY};
                double dL{colorMap[posPixel].lightness() - colorMap[posNeighbor].lightness()};
                double da{colorMap[posPixel].a() - colorMap[posNeighbor].a()};
                double db{colorMap[posPixel].b() - colorMap[posNeighbor].b()};
                double distance{dx * dx + dy * dy + dL * dL + da * da + db * db};
                if (densityMap[posNeighbor] > densityMap[posPixel] && distance < minDistance) {
                    selectedPos = posNeighbor;
                    minDistance = distance;
                }
            }
        }
    }
    return selectedPos;
}

extern "C" {
    void quickShift(std::uint8_t* inputImage, std::uint8_t* outputImage, int width, int height, double sigma, int kernelSize) {
        std::vector<colorm::Lab> colorMap(width * height);
        std::vector<Node*> nodeMap(width * height);

        for (int i{}; i < width * height; ++i) {
            colorMap[i] = colorm::Lab{colorm::Rgb{
                static_cast<double>(inputImage[3 * i]),
                static_cast<double>(inputImage[3 * i + 1]),
                static_cast<double>(inputImage[3 * i + 2])
            }};
            nodeMap[i] = new Node{i, {}};
        }

        std::vector<double> densityMap(width * height);
        for (int i{}; i < width; ++i) {
            for (int j{}; j < height; ++j) {
                densityMap[i + j * width] = parzenDensity(colorMap.data(), width, height, i, j, sigma, kernelSize);
            }
        }

        std::deque<Node*> densityMaxima;
        for (int i{}; i < width; ++i) {
            for (int j{}; j < height; ++j) {
                int posPixel{i + j * width};
                int posNeighbor{findFittingNeighbor(densityMap.data(), colorMap.data(), width, height, i, j, kernelSize)};
                if (posNeighbor == -1) {
                    densityMaxima.push_back(nodeMap[posPixel]);
                }
                else {
                    nodeMap[posNeighbor]->lowerDensityNeighbors.push_back(nodeMap[posPixel]);
                }
            }
        }

        std::stack<Node*> nodeStack;
        for (Node* densityMaximum : densityMaxima) {
            nodeStack.push(densityMaximum);
            do {
                Node* pixelNode{nodeStack.top()};
                nodeStack.pop();
                outputImage[3 * pixelNode->pixel] = inputImage[3 * densityMaximum->pixel];
                outputImage[3 * pixelNode->pixel + 1] = inputImage[3 * densityMaximum->pixel + 1];
                outputImage[3 * pixelNode->pixel + 2] = inputImage[3 * densityMaximum->pixel + 2];
                for (Node* lowerDensityNeighbor : pixelNode->lowerDensityNeighbors) {
                    nodeStack.push(lowerDensityNeighbor);
                }
            } while (!nodeStack.empty());
        }

        for (int i{}; i < width * height; ++i) {
            delete nodeMap[i];
        }
    }
}

// em++ -O3 -s WASM=1 -s EXPORTED_FUNCTIONS='["_malloc", "_free", "_quickShift"]' --no-entry quickshift.cpp -o quickshift.js -s TOTAL_MEMORY=256MB