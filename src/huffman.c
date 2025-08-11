#include "huffman.h"
#include "utils.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

typedef Tree *(*table_fn)(Tree *);

static table_fn decodeTable[2] = {getLeftTree, getRightTree};

Tree *convertToHuffmanTree(Heap *heap)
{
    while (getSizeHeap(heap) > 1)
    {
        Tree *tree1 = popHeap(heap);
        Tree *tree2 = popHeap(heap);
        Tree *tree3 = createTree(0, getFrequencyTree(tree1) + getFrequencyTree(tree2));
        setLeftTree(tree3, tree1);
        setRightTree(tree3, tree2);
        pushHeap(heap, tree3);
    }

    Tree *huffmanTree = popHeap(heap);
    freeHeap(heap);

    return huffmanTree;
}

void helper_convertHuffmanTreeToTable(Tree *tree, unsigned int *table, unsigned int code)
{
    if (!tree)
        return;

    if (!isLeafTree(tree))
    {
        helper_convertHuffmanTreeToTable(getLeftTree(tree), table, code << 0x01);
        helper_convertHuffmanTreeToTable(getRightTree(tree), table, (code << 0x01) | 0x01);
        return;
    }

    unsigned char value = getValueTree(tree);
    table[value] = code;
}

unsigned int *convertHuffmanTreeToTable(Tree *tree)
{
    unsigned int *table = calloc(ASCII_SIZE, sizeof(unsigned int));

    helper_convertHuffmanTreeToTable(tree, table, 0x01);

    return table;
}

void helper_serializeHuffmanTree(Tree *tree, Bitmap *bitmap)
{
    if (!tree)
        return;

    if (isLeafTree(tree))
    {
        insertLSBBitmap(bitmap, 0x01);
        insertByteBitmap(bitmap, getValueTree(tree));
        return;
    }

    insertLSBBitmap(bitmap, 0x00);
    helper_serializeHuffmanTree(getLeftTree(tree), bitmap);
    helper_serializeHuffmanTree(getRightTree(tree), bitmap);
}

void serializeHuffmanTree(Tree *tree, Bitmap *bitmap)
{
    helper_serializeHuffmanTree(tree, bitmap);
}

int getSerializedHuffmanTreeSize(Tree *tree)
{
    int leafNodesCount = getLeafNodesCountTree(tree);
    return leafNodesCount * 10 - 1;
}

void consumeBit(ReadBuffer *buffer, Bitmap *bitmap, Tree *huffmanTree, Tree **tree)
{
    unsigned char bit = bufferNextBit(buffer);
    Tree *next = *tree;

    if (!isLeafTree(huffmanTree))
        next = decodeTable[bit](next); // caso: raiz da árvore de huffman não é uma folha

    if (!isLeafTree(next))
    {
        *tree = next;
        return;
    }

    unsigned char value = getValueTree(next);
    insertAlignedByteBitmap(bitmap, value);
    *tree = huffmanTree;
}

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer)
{
    int isLeafNode = bufferNextBit(buffer);

    if (isLeafNode)
    {
        unsigned char value = bufferNextByte(buffer);
        return createTree(value, 0);
    }

    Tree *tree = createTree(0, 0);

    setLeftTree(tree, createHuffmanTreeFromFile(buffer));
    setRightTree(tree, createHuffmanTreeFromFile(buffer));

    return tree;
}
