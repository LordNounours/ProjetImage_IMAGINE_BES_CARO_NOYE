#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "image_ppm.h"
#include <cmath>
#include <vector>
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
    double ref_X = 95.047; 
    double ref_Y = 100.000;
    double ref_Z = 108.883;
    double epsilon = 0.008856; 
    double kappa = 903.3; 

    for (int i = 0; i < nTaille; i++) {
        var_R = (double)ImgIn[3*i] / 255.0; 
        var_G = (double)ImgIn[3*i + 1] / 255.0; 
        var_B = (double)ImgIn[3*i + 2] / 255.0; 

        if (var_R > 0.04045) var_R = pow((var_R + 0.055) / 1.055, 2.4);
        else var_R = var_R / 12.92;
        if (var_G > 0.04045) var_G = pow((var_G + 0.055) / 1.055, 2.4);
        else var_G = var_G / 12.92;
        if (var_B > 0.04045) var_B = pow((var_B + 0.055) / 1.055, 2.4);
        else var_B = var_B / 12.92;

        var_R = var_R * 100.0;
        var_G = var_G * 100.0;
        var_B = var_B * 100.0;

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

        ImgLab[3*i] = (OCTET)(2.55 * L); 
        ImgLab[3*i + 1] = (OCTET)(a + 128); 
        ImgLab[3*i + 2] = (OCTET)(b + 128); 
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
float distanceEuclidienne(Point p1, Point p2) {//Potentiellement mettre des poids sur certains attributs , spatiaux ou chromatiques
    float distanceXY=sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2));
    float distanceLab=sqrt(pow(p2.L-p1.L,2)+pow(p2.a-p1.a,2)+pow(p2.b-p1.b,2));
    return distanceXY+distanceLab;
}
void generateCentroidsGrid(vector<Point>& centroids, int nH, int nW, int k,OCTET* ImgIn) {
    int gridSize = ceil(sqrt(k)); //grille carrée
    int cellWidth = nW / gridSize;
    int cellHeight = nH / gridSize;
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int randomX = (i * cellWidth) + (rand() % cellWidth);
            int randomY = (j * cellHeight) + (rand() % cellHeight);
            Point center = {randomX, randomY, ImgIn[3*(j*nW+i)], ImgIn[3*(j*nW+i)+1], ImgIn[3*(j*nW+i)+2]}; //valeurs initiales des points
            centroids.push_back(center);
        }
    }
}
void color(OCTET *ImgIn, OCTET *ImgOut, int nH, int nW, vector<int> classe){
    for(int i=0;i<nW-1;i++){
        for(int j=0;j<nH-1;j++){
            int k = classe[j*nW+i];
            int k2 = classe[j*nW+i+1];//prochain pixel horizontal
            int k3 = classe[(j+1)*nW+i];//prochain pixel vertical
            int color=0;//couleur des bordures
            if(k!=k2){//bordure d'épaisseur 2
                ImgOut[3*(j*nW+i)]=color;
                ImgOut[3*(j*nW+i)+1]=color;
                ImgOut[3*(j*nW+i)+2]=color;
                ImgOut[3*(j*nW+i+1)]=color;
                ImgOut[3*(j*nW+i+1)+1]=color;
                ImgOut[3*(j*nW+i+1)+2]=color;
            }
            if(k!=k3){//bordure d'épaisseur 2
                ImgOut[3*(j*nW+i)]=color;
                ImgOut[3*(j*nW+i)+1]=color;
                ImgOut[3*(j*nW+i)+2]=color;
                ImgOut[3*((j+1)*nW+i)]=color;
                ImgOut[3*((j+1)*nW+i)+1]=color;
                ImgOut[3*((j+1)*nW+i)+2]=color;
            }
        }
    }
}
//Seuil utilisé est sur le nombre de changement de classe de pixel a chaque itération.
void slic(OCTET *ImgIn, OCTET *ImgOut,int nH,int nW,int k,int seuil){
    vector<Point> centroide;
    //ordre des boucles importants que ce soit le même a chaque fois pour ordre dans classe
    vector<int> classe;
    generateCentroidsGrid(centroide,nH,nW,k,ImgIn);
    for(int l=0;l<k;l++){
        std::cout << "Centroide " << l+1 << ": x = " << centroide[l].x << ", y = " << centroide[l].y
         << ", L = " << centroide[l].L << ", a = " << centroide[l].a << ", b = " << centroide[l].b << endl;
    }
    //Initialisation
    //Assignation centroide aux pixels
    for(int i=0;i<nW;i++){
        for(int j=0;j<nH;j++){
            Point pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
            float d=distanceEuclidienne(pixel,centroide[0]);
            int indice=0;
            for(int l=1;l<k;l++){
                if(distanceEuclidienne(pixel,centroide[l])<d){
                    indice=l;
                    d=distanceEuclidienne(pixel,centroide[l]);
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
            for(int l=0;l<k;l++){
               if(classe[j*nW+i]==l){
                    addPoints(centroide[l],pixel);
                    count[l]++;
                    break;
               }
            }
        }
    }
    for(int l=0;l<k;l++) divPoint(centroide[l],count[l]);
    //Itération
    int change=nW*nH;
    while(change>seuil)
    {   
        change=0;
        for(int i=0;i<nW;i++){
            for(int j=0;j<nH;j++){
                Point pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
                float d=distanceEuclidienne(pixel,centroide[0]);
                int indice=0;
                for(int l=1;l<k;l++){
                    if(distanceEuclidienne(pixel,centroide[l])<d){
                        indice=l;
                        d=distanceEuclidienne(pixel,centroide[l]);
                    }
                }
                if(indice!=classe[j*nW+i]) {
                    change++;
                    classe[j*nW+i]=indice;
                }
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
                for(int l=0;l<k;l++){
                    if(classe[j*nW+i]==l){
                            addPoints(centroide[l],pixel);
                            count[l]++;
                            break;
                    }
                }
            }
        }
        for(int l=0;l<k;l++) divPoint(centroide[l],count[l]);
        std::cout<<change<<endl;
    }
     for(int l=0;l<k;l++){
        std::cout << "Centroide " << l+1 << ": x = " << centroide[l].x << ", y = " << centroide[l].y
         << ", L = " << centroide[l].L << ", a = " << centroide[l].a << ", b = " << centroide[l].b << endl;
    }
    color(ImgIn,ImgOut,nH,nW,classe);
}

int main(int argc, char* argv[])
{
    char cNomImgLue[250], cNomImgConv[250],cNomImgOut[250];
    int nH, nW, nTaille,k,s;
  
    if (argc != 6) 
        {
        printf("Usage: ImageIn.ppm ImageConv.ppm ImgOut.ppm k s\n"); 
        exit (1) ;
        }

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgConv);
    sscanf (argv[3],"%s",cNomImgOut);
    sscanf (argv[4],"%d",&k);
    sscanf (argv[5],"%d",&s);


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
    slic(ImgConv,ImgOut,nH,nW,k,s);
    ecrire_image_ppm(cNomImgOut, ImgOut,  nH, nW);
    free(ImgIn);
    free(ImgConv);
    free(ImgOut);
    return 1;
}