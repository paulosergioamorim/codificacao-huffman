#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "tree.h"

struct tree
{
    Tree *parent;
    Tree *left;
    Tree *right;
    unsigned char value;
    int frequency;
};

Tree *createTree(unsigned char value, int frequency)
{
    Tree *tree = malloc(sizeof(Tree));
    assert(tree);
    tree->parent = tree->left = tree->right = NULL;
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

void setParentTree(Tree *tree, Tree *parent)
{
    assert(tree);
    tree->parent = parent;
}

Tree *getParentTree(Tree *tree)
{
    assert(tree);
    return tree->parent;
}

int isRootTree(Tree *tree)
{
    assert(tree);
    return !tree->parent;
}

int isLeafTree(Tree *tree)
{
    assert(tree);
    return !tree->left && !tree->right;
}

int getAllNodesCount(Tree *tree)
{
    if (!tree)
        return 0;

    return 1 + getAllNodesCount(tree->left) + getAllNodesCount(tree->right);
}

int getLeafNodesCount(Tree *tree)
{
    if (!tree)
        return 0;

    if (isLeafTree(tree))
        return 1;

    return getLeafNodesCount(tree->left) + getLeafNodesCount(tree->right);
}

char getValueTree(Tree *tree)
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

int compareTrees(Tree *tree1, Tree *tree2)
{
    return tree1->frequency >= tree2->frequency;
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

void printTree(Tree *tree)
{
    if (!tree)
        return;

    printf("<");

    if (isLeafTree(tree))
        printf("(%c, %d)", tree->value, tree->frequency);
    else
        printf("%d", tree->frequency);

    printTree(tree->left);
    printTree(tree->right);

    printf(">");
}

int existsTree(Tree *tree, char value)
{
    if (!tree)
        return 0;

    if (isLeafTree(tree))
        return tree->value == value;

    return existsTree(tree->left, value) || existsTree(tree->right, value);
}

Tree *findTree(Tree *tree, char value)
{
    if (!tree)
        return NULL;

    if (isLeafTree(tree))
        return tree->value == value ? tree : NULL;

    Tree *leftFind = findTree(tree->left, value);
    Tree *rightFind = findTree(tree->right, value);

    return leftFind ? leftFind : rightFind;
}

int encodeLeafTree(Tree *tree)
{
    int encode = 1;

    while (!isRootTree(tree))
    {
        encode = encode << 1;
        Tree *parent = tree->parent;

        if (parent->right == tree)
            encode = encode | 1;

        tree = tree->parent;
    }

    return encode;
}

void helper_convertHuffmanTreeToTable(Tree *tree, unsigned char **table)
{
    if (!tree)
        return;

    if (!isLeafTree(tree))
    {
        helper_convertHuffmanTreeToTable(tree->left, table);
        helper_convertHuffmanTreeToTable(tree->right, table);
        return;
    }

    int encodedChar = encodeLeafTree(tree);
    int i = 0;

    for (; encodedChar != 1; i++)
    {
        table[tree->value][i] = encodedChar & 1;
        encodedChar = encodedChar >> 1;
    }
}

unsigned char **convertHuffmanTreeToTable(Tree *tree)
{
    int height = getHeightTree(tree);
    unsigned char **table = malloc(UCHAR_MAX * sizeof(char *));
    assert(table);

    for (int i = 0; i < UCHAR_MAX; i++)
    {
        table[i] = malloc(height * sizeof(char));
        memset(table[i], 2, height);
        assert(table[i]);
    }

    helper_convertHuffmanTreeToTable(tree, table);

    return table;
}
