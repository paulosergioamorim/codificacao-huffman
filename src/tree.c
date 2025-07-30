#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "tree.h"

struct tree
{
    Tree *left;
    Tree *right;
    unsigned char value;
    int frequency;
};

Tree *createTree(unsigned char value, int frequency)
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
    assert(tree);
    tree->left = left;
}

Tree *getLeftTree(Tree *tree)
{
    assert(tree);
    return tree->left;
}

void setRightTree(Tree *tree, Tree *right)
{
    assert(tree);
    tree->right = right;
}

Tree *getRightTree(Tree *tree)
{
    assert(tree);
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

unsigned char getValueTree(Tree *tree)
{
    assert(tree);
    return tree->value;
}

int getFrequencyTree(Tree *tree)
{
    assert(tree);
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
    assert(tree1);
    assert(tree2);
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
