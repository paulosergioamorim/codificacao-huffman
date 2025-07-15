#pragma once
#include "bitmap.h"

typedef struct tree Tree;

Tree *CreateTree(int value, int frequency, Tree *left, Tree *right);

Tree *GetLeftTree(Tree *tree);

Tree *GetRightTree(Tree *tree);

int GetValueTree(Tree *tree);

int GetFrequencyTree(Tree *tree);

int IsLeafTree(Tree *tree);

void FreeTree(Tree *tree);

void PrintTree(Tree *tree);

int GetHeightTree(Tree *tree);

int GetLeafCount(Tree *tree);

int GetNodesCount(Tree *tree);

bitmap *EncodeHuffmanTree(Tree *huffmanTree);

bitmap *EncodeCharHuffmanTree(Tree *huffmanTree, char encodingChar);

char DecodeCharHuffmanTree(Tree *huffmanTree, int encodedChar);