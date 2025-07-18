#pragma once

typedef struct tree Tree;

Tree *createTree(unsigned char value, int frequency);

void setLeftTree(Tree *tree, Tree *left);

Tree *getLeftTree(Tree *tree);

void setRightTree(Tree *tree, Tree *right);

Tree *getRightTree(Tree *tree);

void setParentTree(Tree *tree, Tree *parent);

Tree *getParentTree(Tree *tree);

int isRootTree(Tree *tree);

int isLeafTree(Tree *tree);

int getAllNodesCount(Tree *tree);

int getLeafNodesCount(Tree *tree);

char getValueTree(Tree *tree);

int getFrequencyTree(Tree *tree);

void freeTree(Tree *tree);

int compareTrees(Tree *tree1, Tree *tree2);

int getHeightTree(Tree *tree);

void printTree(Tree *tree);

int existsTree(Tree *tree, char value);

Tree *findTree(Tree *tree, char value);

int encodeLeafTree(Tree *tree);

unsigned char **convertHuffmanTreeToTable(Tree *tree);
