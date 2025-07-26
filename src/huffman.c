#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "huffman.h"
#include "bitreader.h"

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

void helper_convertHuffmanTreeToTable(Tree *tree, BitArray **table, unsigned int code, int bitsCount)
{
    if (!tree)
        return;

    if (!isLeafTree(tree))
    {
        helper_convertHuffmanTreeToTable(getLeftTree(tree), table, code << 1, bitsCount + 1);
        helper_convertHuffmanTreeToTable(getRightTree(tree), table, (code << 1) | 0x01, bitsCount + 1);
        return;
    }

    unsigned char value = getValueTree(tree);
    table[value] = createStaticBitArray(bitsCount);
    assert(table[value]);

    for (int i = bitsCount - 1; i >= 0; i--)
        insertLSBBitArray(table[value], (unsigned char)(code >> i));
}

BitArray **convertHuffmanTreeToTable(Tree *tree)
{
    assert(tree);
    BitArray **table = calloc(UCHAR_MAX + 1, sizeof(BitArray *));
    assert(table);

    helper_convertHuffmanTreeToTable(tree, table, 0, 0);

    return table;
}

void freeEncodingTable(BitArray **table)
{
    assert(table);
    for (int i = 0; i <= UCHAR_MAX; i++)
        if (table[i])
            freeBitArray(table[i]);

    free(table);
}

void helper_serializeHuffmanTree(Tree *tree, BitArray *array)
{
    if (!tree)
        return;

    if (isLeafTree(tree))
    {
        insertLSBBitArray(array, 0x01);
        insertByteBitArray(array, getValueTree(tree));
        return;
    }

    insertLSBBitArray(array, 0x00);
    helper_serializeHuffmanTree(getLeftTree(tree), array);
    helper_serializeHuffmanTree(getRightTree(tree), array);
}

void serializeHuffmanTree(Tree *tree, BitArray *array)
{
    assert(tree);
    assert(array);
    helper_serializeHuffmanTree(tree, array);
}

int getSerializedHuffmanTreeSize(Tree *tree)
{
    return getNodesCountTree(tree) + 8 * (getLeafNodesCountTree(tree));
}

double helper_getExpectedHeight(long totalBytes, Tree *tree, int height)
{
    if (!tree)
        return 0;

    if (isLeafTree(tree))
        return (double)(height * getFrequencyTree(tree)) / totalBytes;

    return helper_getExpectedHeight(totalBytes, getLeftTree(tree), height + 1) + helper_getExpectedHeight(totalBytes, getRightTree(tree), height + 1);
}

double getExpectedHeightHuffmanTree(long totalBytes, Tree *tree)
{
    return helper_getExpectedHeight(totalBytes, tree, 0);
}
