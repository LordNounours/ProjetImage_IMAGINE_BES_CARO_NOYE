#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "image_ppm.h"
#include <cmath>
#include <vector>
#include <time.h>
using namespace std;


void reduc_rgb_Ycrcb(OCTET *ImgIn, OCTET *ImgOut, int nTaille, int nW,OCTET *ImgY,OCTET *pgmCr,OCTET *pgmCb) {
    //passage en Ycrcb
    for(int i=0;i<nTaille;i++){
        ImgOut[3*i]=0.299*ImgIn[3*i]+0.587*ImgIn[3*i+1]+0.114*ImgIn[3*i+2];
        ImgOut[3*i+1]=-0.1687*ImgIn[3*i]-0.3313*ImgIn[3*i+1]+0.5*ImgIn[3*i+2]+128;
        ImgOut[3*i+2]=0.5*ImgIn[3*i]-0.4187*ImgIn[3*i+1]-0.0813*ImgIn[3*i+2]+128;
    }
}
void convert_rgb_to_lab(OCTET *ImgIn, OCTET *ImgLab, int nTaille) {
    double var_R, var_G, var_B, X, Y, Z, f_X, f_Y, f_Z;
    double var_X, var_Y, var_Z, L, a, b;
    double ref_X = 95.047; // Observer = 2°, Illuminant = D65
    double ref_Y = 100.000;
    double ref_Z = 108.883;
    double epsilon = 0.008856; // 6/29
    double kappa = 903.3; // 29/3 * (29/6) ^ 2

    for (int i = 0; i < nTaille; i++) {
        var_R = (double)ImgIn[3*i] / 255.0; // R from 0 to 255
        var_G = (double)ImgIn[3*i + 1] / 255.0; // G from 0 to 255
        var_B = (double)ImgIn[3*i + 2] / 255.0; // B from 0 to 255

        if (var_R > 0.04045) var_R = pow((var_R + 0.055) / 1.055, 2.4);
        else var_R = var_R / 12.92;
        if (var_G > 0.04045) var_G = pow((var_G + 0.055) / 1.055, 2.4);
        else var_G = var_G / 12.92;
        if (var_B > 0.04045) var_B = pow((var_B + 0.055) / 1.055, 2.4);
        else var_B = var_B / 12.92;

        var_R = var_R * 100.0;
        var_G = var_G * 100.0;
        var_B = var_B * 100.0;

        // Observer = 2°, Illuminant = D65
        X = var_R * 0.4124564 + var_G * 0.3575761 + var_B * 0.1804375;
        Y = var_R * 0.2126729 + var_G * 0.7151522 + var_B * 0.0721750;
        Z = var_R * 0.0193339 + var_G * 0.1191920 + var_B * 0.9503041;

        var_X = X / ref_X;
        var_Y = Y / ref_Y;
        var_Z = Z / ref_Z;

        if (var_X > epsilon) f_X = pow(var_X, 1.0 / 3.0);
        else f_X = (kappa * var_X + 16.0) / 116.0;
        if (var_Y > epsilon) f_Y = pow(var_Y, 1.0 / 3.0);
        else f_Y = (kappa * var_Y + 16.0) / 116.0;
        if (var_Z > epsilon) f_Z = pow(var_Z, 1.0 / 3.0);
        else f_Z = (kappa * var_Z + 16.0) / 116.0;

        L = (116.0 * f_Y) - 16.0;
        a = 500.0 * (f_X - f_Y);
        b = 200.0 * (f_Y - f_Z);

        // Normalize values to fit in [0, 255]
        ImgLab[3*i] = (OCTET)(2.55 * L); // L from 0 to 100
        ImgLab[3*i + 1] = (OCTET)(a + 128); // a from -128 to 127
        ImgLab[3*i + 2] = (OCTET)(b + 128); // b from -128 to 127
    }
}
struct Point{
    int x;
    int y;
    int L;
    int a;
    int b;
};
void resetVectorPoint(std::vector<Point>& vecteur) {
    for (int i = 0; i < vecteur.size(); ++i) {
        vecteur[i] = {0,0,0,0,0};
    }
}
void addPoints(Point& point1, const Point& point2) {
    point1.x += point2.x;
    point1.y += point2.y;
    point1.L += point2.L;
    point1.a += point2.a;
    point1.b += point2.b;
}
void divPoint(Point& point, int diviseur) {
    point.x /= diviseur;
    point.y /= diviseur;
    point.L /= diviseur;
    point.a /= diviseur;
    point.b /= diviseur;
}
float distanceEuclidienne(Point p1, Point p2) {
    float distanceXY=sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2));
    float distanceLab=sqrt(pow(p2.L-p1.L,2)+pow(p2.a-p1.a,2)+pow(p2.b-p1.b,2));
    return distanceXY+distanceLab;
}
void slic(OCTET *ImgIn, OCTET *ImgOut,int nH,int nW,int k){
    vector<Point> centroide;
    //ordre des boucles importants que ce soit le même a chaque fois pour ordre dans classe
    vector<int> classe;
    srand(time(NULL));
    for(int i=0;i<k;i++){
        int randomX = rand() % (nW + 1);
        int randomY = rand() % (nH + 1);
        Point center={randomX,randomY,ImgIn[(randomY*nW+randomX)*3],ImgIn[(randomY*nW+randomX)*3+1],ImgIn[(randomY*nW+randomX)*3+2]};
        centroide.push_back(center);
    }
    //Initialisation
    //Assignation centroide aux pixels
    for(int i=0;i<nW;i++){
        for(int j=0;j<nH;j++){
            Point pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
            float d=distanceEuclidienne(pixel,centroide[0]);
            int indice=0;
            for(int k=1;k<k;k++){
                if(distanceEuclidienne(pixel,centroide[k])<d){
                    indice=k;
                    d=distanceEuclidienne(pixel,centroide[k]);
                }
            }
            classe.push_back(indice);
        }
    }
    //Recalcule des nouveaux centroides
    //Mise a 0 des valeurs de centroides
    int count[k]={0}; // Déclaration d'un tableau de taille k
    resetVectorPoint(centroide);
    for(int i=0;i<nW;i++){
        for(int j=0;j<nH;j++){
            Point pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
            float d=distanceEuclidienne(pixel,centroide[0]);
            int indice=0;
            for(int k=1;k<k;k++){
               if(classe[j*nW+i]==k){
                    addPoints(centroide[k],pixel);
                    count[k]++;
               }
            }
        }
    }
    for(int k=0;k<k;k++) divPoint(centroide[k],count[k]);
    //Itération
}

int main(int argc, char* argv[])
{
    char cNomImgLue[250], cNomImgConv[250],cNomImgOut[250];
    int nH, nW, nTaille,k;
  
    if (argc != 5) 
        {
        printf("Usage: ImageIn.ppm ImageConv.ppm ImgOut.ppm k\n"); 
        exit (1) ;
        }

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgConv);
    sscanf (argv[3],"%s",cNomImgOut);
    sscanf (argv[4],"%d",&k);


    OCTET *ImgIn, *ImgConv,*ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, 3*nTaille);
    allocation_tableau(ImgConv, OCTET, 3*nTaille);
    allocation_tableau(ImgOut, OCTET, 3*nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    lire_image_ppm(cNomImgLue, ImgOut, nH * nW);
    convert_rgb_to_lab(ImgIn,ImgConv,nTaille);
    ecrire_image_ppm(cNomImgConv, ImgConv,  nH, nW);
    slic(ImgConv,ImgOut,nH,nW,k);
    free(ImgIn);
    return 1;
}