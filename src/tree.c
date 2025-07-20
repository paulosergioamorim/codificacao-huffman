#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "bitreader.h"
#include "tree.h"

struct tree
{
    Tree *left;
    Tree *right;
    byte value;
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

byte getValueTree(Tree *tree)
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
        printf("%c", tree->value);

    printTree(tree->left);
    printTree(tree->right);

    printf(">");
}

void helper_convertHuffmanTreeToTable(Tree *tree, ArrayByte **table, int code, int bitsCount)
{
    if (!tree)
        return;

    if (!isLeafTree(tree))
    {
        helper_convertHuffmanTreeToTable(tree->left, table, code << 1, bitsCount + 1);
        helper_convertHuffmanTreeToTable(tree->right, table, (code << 1) | 0x01, bitsCount + 1);
        return;
    }

    table[tree->value] = createArrayByte(bitsCount);
    assert(table[tree->value]);

    for (int i = bitsCount - 1; i >= 0; i--)
        insertLSBArrayByte(table[tree->value], code >> i);
}

ArrayByte **convertHuffmanTreeToTable(Tree *tree)
{
    ArrayByte **table = calloc(UCHAR_MAX, sizeof(ArrayByte *));
    assert(table);

    helper_convertHuffmanTreeToTable(tree, table, 1, 0);

    return table;
}

void freeEncodingTable(ArrayByte **table)
{
    for (int i = 0; i < UCHAR_MAX; i++)
        if (table[i])
            freeArrayByte(table[i]);

    free(table);
}

void helper_dumpHuffmanTree(Tree *tree, ArrayByte *array)
{
    if (!tree)
        return;

    if (isLeafTree(tree))
    {
        insertLSBArrayByte(array, 0x01);
        insertByteArrayByte(array, tree->value);
        return;
    }

    insertLSBArrayByte(array, 0x00);
    helper_dumpHuffmanTree(tree->left, array);
    helper_dumpHuffmanTree(tree->right, array);
}

void saveHuffmanTreeToFile(Tree *tree, FILE *fp)
{
    int allNodesCount = getAllNodesCount(tree);
    int leafNodesCount = getLeafNodesCount(tree);
    int bitmapSize = allNodesCount + leafNodesCount * 8; // (allNodesCount - leafNodesCount) * 1 + leafNodesCount * 9
    ArrayByte *array = createArrayByte(bitmapSize);
    helper_dumpHuffmanTree(tree, array);
    fwrite(&leafNodesCount, 1, sizeof(int), fp);
    fwrite(getContentArrayByte(array), getBytesLengthArrayByte(array), sizeof(byte), fp);
    freeArrayByte(array);
}

Tree *helper_buildHuffmanTreeFromFile(int leafNodesCount, int currentLeafNodesCount, BitReader *br)
{
    if (leafNodesCount == currentLeafNodesCount)
        return NULL;

    int isLeafNode = readBitBitReader(br);

    if (isLeafNode)
    {
        byte value = readByteBitReader(br);
        return createTree(value, 0);
    }

    Tree *tree = createTree(0, 0);

    tree->left = helper_buildHuffmanTreeFromFile(leafNodesCount, currentLeafNodesCount, br);
    tree->right = helper_buildHuffmanTreeFromFile(leafNodesCount, currentLeafNodesCount, br);

    return tree;
}

Tree *createHuffmanTreeFromFile(FILE *fp)
{
    int leafNodesCount = 0;
    fread(&leafNodesCount, 1, sizeof(int), fp);
    BitReader *br = createBitReader(fp);
    Tree *huffmanTree = helper_buildHuffmanTreeFromFile(leafNodesCount, 0, br);
    freeBitReader(br);
    return huffmanTree;
}

// Tree *createHuffmanTreeFromFile(FILE *fp)
// {
//     int leafNodesCount = 0;
//     fread(&leafNodesCount, 1, sizeof(int), fp);

//     Tree *huffmanTree = NULL;
//     Tree *cur = NULL;
//     int j = 0; // index of bit

//     byte buffer = 0;
//     fread(&buffer, 1, sizeof(byte), fp);

//     for (int i = 0; i < leafNodesCount; i++)
//     {
//         if (buffer == EOF)
//             break;

//         while ((buffer >> (7 - j) & 0x01) == 0)
//         {
//             j++;

//             if (!huffmanTree)
//             {
//                 huffmanTree = createTree(0, 0);
//                 cur = huffmanTree;
//                 printTree(huffmanTree);
//                 printf("\n");
//                 continue;
//             }

//             Tree *tree = createTree(0, 0);
//             assert((cur = insertNode(cur, tree)));
//             printTree(huffmanTree);
//             printf("\n");
//         }

//         byte value = 0;
//         j = (j + 1) % 8;
//         value = buffer << j;
//         fread(&buffer, 1, sizeof(byte), fp);

//         if (j == 0)
//         {
//             value = buffer;
//             fread(&buffer, 1, sizeof(byte), fp);
//             continue;
//         }

//         value |= buffer >> (8 - j);

//         Tree *tree = createTree(value, 0);
//         assert((cur = insertNode(cur, tree)));
//         printTree(huffmanTree);
//         printf("\n");
//     }

//     return huffmanTree;
// }