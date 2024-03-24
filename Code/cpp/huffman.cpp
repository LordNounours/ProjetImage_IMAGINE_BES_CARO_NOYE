#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;
#include <queue>
#include <unordered_map>

struct HuffmanNode {
    char data; // Valeur du composant
    unsigned frequency; // Fréquence du composant
    HuffmanNode *left, *right; // Pointeurs vers les noeuds enfants

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->frequency > r->frequency;
    }
};

void deleteTree(HuffmanNode* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

HuffmanNode* buildHuffmanTree(const unordered_map<char, unsigned>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for (auto& pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() != 1) {
        HuffmanNode *left = pq.top(); pq.pop();
        HuffmanNode *right = pq.top(); pq.pop();

        HuffmanNode *newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        pq.push(newNode);
    }

    return pq.top();
}

void generateCodes(HuffmanNode* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr) return;

    if (root->data != '$') {
        codes[root->data] = code;
    }

    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

string encode(const unsigned char* ImgIn, int size, unordered_map<char, string>& codes) {
    string encodedText = "";
    for (int i=0;i<size;++i) {
        encodedText += codes[ImgIn[i]];
    }
    return encodedText;
}

int main(int argc, char* argv[])
{
    char cNomImgLue[250];
    int nH, nW, imgSize, imgCompresseeSize;
  
    if(argc != 2){
        printf("Usage: ImageIn.ppm\n"); 
        exit(1);
    }

    sscanf(argv[1],"%s",cNomImgLue);

    unsigned char *ImgIn;
    int channels;
    ImgIn = stbi_load(cNomImgLue, &nW, &nH, &channels, STBI_rgb);
    if (ImgIn == NULL) {
        std::cerr << "Erreur lors du chargement des images." << std::endl;
        return 1;
    }
    imgSize = (nH * nW * 3);

    unordered_map<char, unsigned> freqMap;

    for (int i=0;i<imgSize;++i) {
        freqMap[ImgIn[i]]++;
    }

    // Construction de l'arbre Huffman
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Génération des codes Huffman
    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    string ImgCompressee = encode(ImgIn,imgSize, codes).c_str();
    imgCompresseeSize = ImgCompressee.size()/8; //convertion en octet

    cout << "[INFO] Taille image origine : " << imgSize << " octets" << endl;
    cout << "[INFO] Taille image compressee (HUFFMAN) : " << imgCompresseeSize << " octets" << endl;
    cout << "[INFO] Taux de compression : " << (float)imgSize/imgCompresseeSize << endl;

    deleteTree(root);

    stbi_image_free(ImgIn);
    return 1;
}