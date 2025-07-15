#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"

struct tree
{
    char value;
    int frequency;
    Tree *left;
    Tree *right;
};

static void inner_EncodeCharHuffmanTree(Tree *tree, char encodingChar, bitmap *bm);
void bitmapAppendByte(bitmap *bm, unsigned char byte);

Tree *CreateTree(int value, int frequency, Tree *left, Tree *right)
{
    Tree *tree = malloc(sizeof(Tree));
    assert(tree);
    tree->left = left;
    tree->right = right;
    tree->value = value;
    tree->frequency = frequency;

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

int GetFrequencyTree(Tree *tree)
{
    assert(tree);
    return tree->frequency;
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
        printf("%c (%d)", GetValueTree(tree), GetFrequencyTree(tree));
    else
        printf("%d ", GetFrequencyTree(tree));
    PrintTree(tree->left);
    PrintTree(tree->right);
    printf(">");
}

int GetHeightTree(Tree *tree)
{
    if (!tree)
        return 0;

    int leftHeight = 1 + GetHeightTree(tree->left);
    int rightHeight = 1 + GetHeightTree(tree->right);

    if (leftHeight >= rightHeight)
        return leftHeight;

    return rightHeight;
}

int ExistsTree(Tree *tree, char key)
{
    if (!tree)
        return 0;

    if (tree->value == key)
        return 1;

    return ExistsTree(tree->left, key) || ExistsTree(tree->right, key);
}

static void inner_EncodeCharHuffmanTree(Tree *tree, char encodingChar, bitmap *bm)
{
    if (IsLeafTree(tree) && tree->value == encodingChar)
        return;

    if (ExistsTree(tree->left, encodingChar))
    {
        bitmapAppendLeastSignificantBit(bm, 0b0);
        inner_EncodeCharHuffmanTree(tree->left, encodingChar, bm);
        return;
    }

    bitmapAppendLeastSignificantBit(bm, 0b1);
    inner_EncodeCharHuffmanTree(tree->right, encodingChar, bm);
}

bitmap *EncodeCharHuffmanTree(Tree *tree, char c)
{
    bitmap *bm = bitmapInit(GetHeightTree(tree));
    inner_EncodeCharHuffmanTree(tree, c, bm);
    return bm;
}

char DecodeCharHuffmanTree(Tree *huffmanTree, int encodedChar)
{
    if (IsLeafTree(huffmanTree))
        return GetValueTree(huffmanTree);

    int moveToLeft = encodedChar % 2 == 0;
    encodedChar = encodedChar >> 1;

    return DecodeCharHuffmanTree(moveToLeft ? huffmanTree->left : huffmanTree->right, encodedChar);
}

int GetLeafCount(Tree *tree)
{
    if (!tree)
        return 0;

    if (IsLeafTree(tree))
        return 1;

    return GetLeafCount(tree->left) + GetLeafCount(tree->right);
}

int GetNodesCount(Tree *tree)
{
    if (!tree)
        return 0;

    return 1 + GetNodesCount(tree->left) + GetNodesCount(tree->right);
}

static void inner_EncodeHuffmanTree(Tree *tree, bitmap *bm);

static void inner_EncodeHuffmanTree(Tree *tree, bitmap *bm)
{
    if (!tree)
        return;

    if (IsLeafTree(tree))
    {
        bitmapAppendLeastSignificantBit(bm, 0b1);
        bitmapAppendByte(bm, tree->value);
        return;
    }

    bitmapAppendLeastSignificantBit(bm, 0b0);
    inner_EncodeHuffmanTree(tree->left, bm);
    inner_EncodeHuffmanTree(tree->right, bm);
}

void bitmapAppendByte(bitmap *bm, unsigned char byte)
{
    for (int i = 7; i >= 0; i--)
    {
        unsigned char bit = (byte >> i) & 0x01;
        bitmapAppendLeastSignificantBit(bm, bit);
    }
}

bitmap *EncodeHuffmanTree(Tree *huffmanTree)
{
    bitmap *bm = bitmapInit(GetLeafCount(huffmanTree) * 8 + GetNodesCount(huffmanTree));
    inner_EncodeHuffmanTree(huffmanTree, bm);
    return bm;
}
