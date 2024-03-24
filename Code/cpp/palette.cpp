#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <map> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

struct Point {
    int r;
    int g;
    int b;

    bool operator==(const Point& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
     bool operator<(const Point& other) const {
        if (r != other.r)
            return r < other.r;
        if (g != other.g)
            return g < other.g;
        return b < other.b;
    }
};

std::vector<Point> creerPalette(unsigned char *ImgSeg, int nH, int nW) {
    std::vector<Point> palette;
    Point pixel;
    for (std::size_t i = 0; i < nH; i++) {
        for (std::size_t j = 0; j < nW; j++) {
            pixel = {ImgSeg[(i * nW + j) * 3], ImgSeg[(i * nW + j) * 3 + 1], ImgSeg[(i * nW + j) * 3 + 2]};
            bool found = false;
            for (const Point& p : palette) {
                if (p == pixel) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                palette.push_back(pixel);
            }
        }
    }
    return palette;
}

void compresserImage(unsigned char *ImgIn, unsigned char *ImgOut, int nH, int nW, const std::vector<Point>& palette) {
    std::map<Point, int> paletteIndexMap;
    for (std::size_t i = 0; i < palette.size(); ++i) {
        paletteIndexMap[palette[i]] = i;
    }
    for (std::size_t i = 0; i < nH; i++) {
        for (std::size_t j = 0; j < nW; j++) {
            Point pixel = {ImgIn[(i * nW + j) * 3], ImgIn[(i * nW + j) * 3 + 1], ImgIn[(i * nW + j) * 3 + 2]};
            int index = paletteIndexMap[pixel];
            ImgOut[i * nW + j] = index;
        }
    }
}

void compression(unsigned char *ImgIn, unsigned char *ImgSeg, unsigned char *ImgOut, int nH, int nW) {
    std::vector<Point> palette = creerPalette(ImgSeg, nH, nW);
    compresserImage(ImgIn, ImgOut, nH, nW, palette);
}

int main(int argc, char* argv[]) {
    char cNomImg[250], cNomImgSeg[250], cNomImgOut[250];
    int nH, nW, nTaille;

    if (argc != 4) {
        printf("Usage: ImageIn.ppm ImageConv.ppm ImgOut.ppm k s\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImg);
    sscanf(argv[2], "%s", cNomImgSeg);
    sscanf(argv[3], "%s", cNomImgOut);

    unsigned char *ImgIn, *ImgSeg, *ImgOut;
    int channels;
    ImgIn = stbi_load(cNomImg, &nW, &nH, &channels, STBI_rgb);
    if (ImgIn == NULL) {
        std::cerr << "Erreur lors du chargement de l'image " << cNomImg << std::endl;
        return 1;
    }
    nTaille = nH * nW;
    ImgSeg = stbi_load(cNomImgSeg, &nW, &nH, &channels, STBI_rgb);
    if (ImgSeg == NULL) {
        std::cerr << "Erreur lors du chargement de l'image " << cNomImgSeg << std::endl;
        return 1;
    }
    ImgOut = (unsigned char *)malloc(nTaille * sizeof(unsigned char));
    compression(ImgIn, ImgSeg, ImgOut, nH, nW);

    stbi_write_png(cNomImgOut, nW, nH, 1, ImgOut, nW);
    stbi_image_free(ImgIn);
    stbi_image_free(ImgSeg);
    free(ImgOut);
    return 0;
}
