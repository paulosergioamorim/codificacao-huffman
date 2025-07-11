#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

struct tree
{
    char value;
    int weigth;
    Tree *left;
    Tree *right;
};

Tree *CreateTree(int value, int weigth)
{
    Tree *tree = malloc(sizeof(Tree));
    assert(tree);
    tree->left = tree->right = NULL;
    tree->value = value;
    tree->weigth = weigth;

    return tree;
}

Tree *SetLeftTree(Tree *tree, Tree *left)
{
    assert(tree);
    tree->left = left;
    return tree;
}

Tree *SetRightTree(Tree *tree, Tree *right)
{
    assert(tree);
    tree->right = right;
    return tree;
}

Tree *GetLeftTree(Tree *tree)
{
    assert(tree);
    return tree->right;
}

Tree *GetRightTree(Tree *tree)
{
    assert(tree);
    return tree->left;
}

int GetValueTree(Tree *tree)
{
    assert(tree);
    return tree->value;
}

int GetWeigthTree(Tree *tree)
{
    assert(tree);
    return tree->weigth;
}

int IsLeafTree(Tree *tree)
{
    assert(tree);
    return !tree->left && !tree->right;
}

void FreeTree(Tree *tree)
{
    if (!tree)
        return;

    FreeTree(tree->left);
    FreeTree(tree->right);
    free(tree);
}

void PrintTree(Tree *tree)
{
    printf("<");
    if (!tree)
        return;
    if (IsLeafTree(tree))
        printf("%c ", GetValueTree(tree));
    else
        printf("%d ", GetWeigthTree(tree));
    PrintTree(tree->left);
    PrintTree(tree->right);
    printf(">");
}