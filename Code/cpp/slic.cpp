#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // Inclure le fichier d'en-tête stb_image.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace std;



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
void generateCentroidsGrid(vector<Point>& centroids, int nH, int nW, int k,unsigned char* ImgIn) {
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
void color(unsigned char *ImgIn, unsigned char *ImgOut, int nH, int nW, vector<int> classe){ 
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

void color2(unsigned char *ImgIn, unsigned char *ImgOut, int nH, int nW, vector<int> classe,vector<Point> centroide){
    for(int i=0;i<nW;i++){
        for(int j=0;j<nH;j++){
            int k = classe[j*nW+i];
            ImgOut[3*(j*nW+i)]=centroide[k].L;
            ImgOut[3*(j*nW+i)+1]=centroide[k].a;
            ImgOut[3*(j*nW+i)+2]=centroide[k].b;
        }
    }
}
//Seuil utilisé est sur le nombre de changement de classe de pixel a chaque itération.
void slic(unsigned char *ImgIn, unsigned char *ImgOut,int nH,int nW,int k,int seuil){
    vector<Point> centroide;
    vector<Point> centroideTmp(k);
    //ordre des boucles importants que ce soit le même a chaque fois pour ordre dans classe
    vector<int> classe;
    generateCentroidsGrid(centroide,nH,nW,k,ImgIn);
    //Initialisation
    //Assignation centroide aux pixels
    int indice;
    Point pixel;
    float d;
    for(int i=0;i<nW;i++){
        for(int j=0;j<nH;j++){
            pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
            d=distanceEuclidienne(pixel,centroide[0]);
            indice=0;
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
    resetVectorPoint(centroideTmp);
    for(int i=0;i<nW;i++){
        for(int j=0;j<nH;j++){
            pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
            d=distanceEuclidienne(pixel,centroide[0]);
            int indice=0;
            for(int l=0;l<k;l++){
               if(classe[j*nW+i]==l){
                    addPoints(centroideTmp[l],pixel);
                    count[l]++;
                    break;
               }
            }
        }
    }
    for(int l=0;l<k;l++) {
            if(count[l]!=0)divPoint(centroideTmp[l],count[l]);
    }
    centroide=centroideTmp;
    //Itération
    int change=nW*nH;
    while(change>seuil)
    {   
        change=0;
        for(int i=0;i<nW;i++){
            for(int j=0;j<nH;j++){
                pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
                d=distanceEuclidienne(pixel,centroide[0]);
                indice=0;
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
        resetVectorPoint(centroideTmp);
        for(int i=0;i<nW;i++){
            for(int j=0;j<nH;j++){
                pixel={i,j,ImgIn[(j*nW+i)*3],ImgIn[(j*nW+i)*3+1],ImgIn[(j*nW+i)*3+2]};
                d=distanceEuclidienne(pixel,centroide[0]);
                indice=0;
                for(int l=0;l<k;l++){
                    if(classe[j*nW+i]==l){
                            addPoints(centroideTmp[l],pixel);
                            count[l]++;
                            break;
                    }
                }
            }
        }
        for(int l=0;l<k;l++) {
            if(count[l]!=0)divPoint(centroideTmp[l],count[l]);
        }
        centroide=centroideTmp;
        std::cout<<change<<endl;
    }
    //color(ImgIn,ImgOut,nH,nW,classe);
    color2(ImgIn,ImgOut,nH,nW,classe,centroide);
}

int main(int argc, char* argv[])
{
    char cNomImgLue[250],cNomImgOut[250];
    int nH, nW, nTaille,k,s;
  
    if (argc != 5) 
        {
        printf("Usage: ImageIn.ppm ImageConv.ppm ImgOut.ppm k s\n"); 
        exit (1) ;
        }

    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgOut);
    sscanf (argv[3],"%d",&k);
    sscanf (argv[4],"%d",&s);


    unsigned char *ImgIn, *ImgConv,*ImgOut;
    int channels;
    ImgIn = stbi_load(cNomImgLue, &nW, &nH, &channels, STBI_rgb);
    if (ImgIn == NULL) {
        std::cerr << "Erreur lors du chargement des images." << std::endl;
        return 1;
    }
    nTaille = nH * nW;
    ImgOut = (unsigned char *)malloc(3 * nTaille * sizeof(unsigned char));

  
    slic(ImgIn,ImgOut,nH,nW,k,s);
    stbi_write_png(cNomImgOut, nW, nH, 3, ImgOut, nW * 3);
    stbi_image_free(ImgIn);
    free(ImgOut);
    return 1;
}