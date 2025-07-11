#pragma once

typedef struct tree Tree;

Tree *CreateTree(int value, int weigth);

Tree *SetLeftTree(Tree *tree, Tree *left);

Tree *SetRightTree(Tree *tree, Tree *right);

Tree *GetLeftTree(Tree *tree);

Tree *GetRightTree(Tree *tree);

int GetValueTree(Tree *tree);

int GetWeigthTree(Tree *tree);

int IsLeafTree(Tree *tree);

void FreeTree(Tree *tree);

void PrintTree(Tree *tree);