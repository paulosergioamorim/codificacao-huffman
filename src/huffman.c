#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "huffman.h"
#include "stack.h"

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
        helper_convertHuffmanTreeToTable(getLeftTree(tree), table, code << 1);
        helper_convertHuffmanTreeToTable(getRightTree(tree), table, (code << 1) | 1);
        return;
    }

    unsigned char value = getValueTree(tree);
    table[value] = code;
}

unsigned int *convertHuffmanTreeToTable(Tree *tree)
{
    assert(tree);
    unsigned int *table = calloc(UCHAR_MAX + 1, sizeof(unsigned int));
    assert(table);

    helper_convertHuffmanTreeToTable(tree, table, 1);

    return table;
}

void freeEncodingTable(unsigned int *table)
{
    assert(table);
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
    int leafNodesCount = getLeafNodesCountTree(tree);
    return leafNodesCount * 10 - 1;
}
