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

int getNodesCountTree(Tree *tree)
{
    if (!tree)
        return 0;

    return 1 + getNodesCountTree(tree->left) + getNodesCountTree(tree->right);
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

int compareTrees(Tree *tree1, Tree *tree2)
{
    assert(tree1);
    assert(tree2);
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