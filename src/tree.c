/**
 * @file tree.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 */

#include "tree.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct tree
{
    Tree *left;
    Tree *right;
    unsigned char value;
    unsigned int frequency;
};

Tree *createTree(unsigned char value, unsigned int frequency)
{
    Tree *tree = malloc(sizeof(Tree));
    assert(tree);
    tree->left = tree->right = NULL;
    tree->value = value;
    tree->frequency = frequency;

    return tree;
}

void setLeftTree(Tree *tree, Tree *left)
{
    tree->left = left;
}

Tree *getLeftTree(Tree *tree)
{
    return tree->left;
}

void setRightTree(Tree *tree, Tree *right)
{
    tree->right = right;
}

Tree *getRightTree(Tree *tree)
{
    return tree->right;
}

int isLeafTree(Tree *tree)
{
    return tree && !tree->left && !tree->right;
}

int getLeafNodesCountTree(Tree *tree)
{
    if (!tree)
        return 0;

    if (isLeafTree(tree))
        return 1;

    return getLeafNodesCountTree(tree->left) + getLeafNodesCountTree(tree->right);
}

int getHeightTree(Tree *tree)
{
    if (!tree)
        return 0;

    int leftHeight = 1 + getHeightTree(tree->left);
    int rightHeight = 1 + getHeightTree(tree->right);

    if (leftHeight >= rightHeight)
        return leftHeight;

    return rightHeight;
}

unsigned char getValueTree(Tree *tree)
{
    return tree->value;
}

unsigned int getFrequencyTree(Tree *tree)
{
    return tree->frequency;
}

void freeTree(Tree *tree)
{
    if (!tree)
        return;

    freeTree(tree->left);
    freeTree(tree->right);
    free(tree);
}

int compareFrequencyTrees(Tree *tree1, Tree *tree2)
{
    return tree1->frequency >= tree2->frequency;
}

void printTree(Tree *tree)
{
    if (!tree)
        return;

    printf("<");

    if (isLeafTree(tree))
        printf("%c", tree->value);

    printTree(tree->left);
    printTree(tree->right);

    printf(">");
}
