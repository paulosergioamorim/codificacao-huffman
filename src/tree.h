#pragma once

typedef struct tree Tree;

Tree *createTree(unsigned char value, int frequency);

void setLeftTree(Tree *tree, Tree *left);

Tree *getLeftTree(Tree *tree);

void setRightTree(Tree *tree, Tree *right);

Tree *getRightTree(Tree *tree);

int isLeafTree(Tree *tree);

int getLeafNodesCountTree(Tree *tree);

unsigned char getValueTree(Tree *tree);

int getFrequencyTree(Tree *tree);

void freeTree(Tree *tree);

int compareTrees(Tree *tree1, Tree *tree2);

void printTree(Tree *tree);
