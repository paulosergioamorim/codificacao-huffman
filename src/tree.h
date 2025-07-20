#pragma once

#include "arraybyte.h"

typedef struct tree Tree;

Tree *createTree(byte value, int frequency);

void setLeftTree(Tree *tree, Tree *left);

Tree *getLeftTree(Tree *tree);

void setRightTree(Tree *tree, Tree *right);

Tree *getRightTree(Tree *tree);

int isLeafTree(Tree *tree);

int getAllNodesCount(Tree *tree);

int getLeafNodesCount(Tree *tree);

byte getValueTree(Tree *tree);

int getFrequencyTree(Tree *tree);

void freeTree(Tree *tree);

int compareTrees(Tree *tree1, Tree *tree2);

int getHeightTree(Tree *tree);

void printTree(Tree *tree);

ArrayByte **convertHuffmanTreeToTable(Tree *tree);

void freeEncodingTable(ArrayByte **table);

void saveHuffmanTreeToFile(Tree *tree, FILE *fp);

Tree *createHuffmanTreeFromFile(FILE *fp);
