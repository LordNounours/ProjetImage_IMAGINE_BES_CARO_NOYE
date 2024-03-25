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
};


double distance(const Point& p1, const Point& p2) {
    return std::sqrt((p1.r - p2.r) * (p1.r - p2.r) +
                     (p1.g - p2.g) * (p1.g - p2.g) +
                     (p1.b - p2.b) * (p1.b - p2.b));
}

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
    std::vector<int> indicePalette;
    for(int i = 0; i < palette.size(); i++) {
        indicePalette.push_back(i);
    }
    for (std::size_t i = 0; i < nH; i++) {
        for (std::size_t j = 0; j < nW; j++) {
            Point pixel = {ImgIn[(i * nW + j) * 3], ImgIn[(i * nW + j) * 3 + 1], ImgIn[(i * nW + j) * 3 + 2]};
            double minDistance = std::numeric_limits<double>::max();
            int indiceMinDistance = -1;
            for(int k = 0; k < palette.size(); k++) {
                double dist = distance(pixel, palette[k]);
                if (dist < minDistance) {
                    minDistance = dist;
                    indiceMinDistance = k;
                }
            }
            if (indiceMinDistance != -1) {
                ImgOut[i * nW + j] = indicePalette[indiceMinDistance];
            } else {
                std::cerr << "Pixel non trouvé dans la palette." << std::endl;
            }
        }
    }
}




void decompresserImage(unsigned char *ImgIn, unsigned char *ImgOut, int nH, int nW, const std::vector<Point>& palette) {
    for (std::size_t i = 0; i < nH; i++) {
        for (std::size_t j = 0; j < nW; j++) {
            int index = ImgIn[i * nW + j];
            ImgOut[(i * nW + j) * 3] = palette[index].r; 
            ImgOut[(i * nW + j) * 3 + 1] = palette[index].g; 
            ImgOut[(i * nW + j) * 3 + 2] = palette[index].b; 
        }
    }
}



void decompression(unsigned char *ImgIn, unsigned char *ImgOut, int nH, int nW, const std::vector<Point>& palette) {
    decompresserImage(ImgIn, ImgOut, nH, nW, palette);
}

void compression(unsigned char *ImgIn, unsigned char *ImgSeg, unsigned char *ImgOut,unsigned char* ImgDec, int nH, int nW) {
    std::vector<Point> palette = creerPalette(ImgSeg, nH, nW);
    for (const Point& p : palette) {
        std::cout << "Couleur : (" << p.r << ", " << p.g << ", " << p.b << ")" << std::endl;
    }
    compresserImage(ImgIn, ImgOut, nH, nW, palette);
    decompression(ImgOut,ImgDec,nH,nW,palette);
}


int main(int argc, char* argv[]) {
    char cNomImg[250], cNomImgSeg[250], cNomImgComp[250], cNomImgDec[250];
    int nH, nW, nTaille;

    if (argc != 5) {
        printf("Usage: ImageIn.ppm ImageConv.ppm ImgOut.ppm k s\n");
        exit(1);
    }

    sscanf(argv[1], "%s", cNomImg);
    sscanf(argv[2], "%s", cNomImgSeg);
    sscanf(argv[3], "%s", cNomImgComp);
    sscanf(argv[4], "%s", cNomImgDec);

    unsigned char *ImgIn, *ImgSeg, *ImgComp,*ImgDec;
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
    ImgComp = (unsigned char *)malloc(nTaille * sizeof(unsigned char));
    ImgDec = (unsigned char *)malloc(nTaille * 3 * sizeof(unsigned char));
    compression(ImgIn, ImgSeg, ImgComp,ImgDec, nH, nW);

    stbi_write_png(cNomImgComp, nW, nH, 1, ImgComp, 0);
    stbi_write_png(cNomImgDec, nW, nH, 3, ImgDec, 0 );
    stbi_image_free(ImgIn);
    stbi_image_free(ImgSeg);
    free(ImgComp);
    free(ImgDec);
    return 0;
}