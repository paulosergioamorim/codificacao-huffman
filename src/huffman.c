#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "huffman.h"
#include "bitreader.h"

void helper_convertHuffmanTreeToTable(Tree *tree, ArrayByte **table, int code, int bitsCount)
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
    table[value] = createArrayByte(bitsCount);
    assert(table[value]);

    for (int i = bitsCount - 1; i >= 0; i--)
        insertLSBArrayByte(table[value], code >> i);
}

ArrayByte **convertHuffmanTreeToTable(Tree *tree)
{
    assert(tree);
    ArrayByte **table = calloc(UCHAR_MAX, sizeof(ArrayByte *));
    assert(table);

    helper_convertHuffmanTreeToTable(tree, table, 1, 0);

    return table;
}

void freeEncodingTable(ArrayByte **table)
{
    assert(table);
    for (int i = 0; i < UCHAR_MAX; i++)
        if (table[i])
            freeArrayByte(table[i]);

    free(table);
}

void helper_saveHuffmanTreeToFile(Tree *tree, ArrayByte *array)
{
    if (!tree)
        return;

    if (isLeafTree(tree))
    {
        insertLSBArrayByte(array, 0x01);
        insertByteArrayByte(array, getValueTree(tree));
        return;
    }

    insertLSBArrayByte(array, 0x00);
    helper_saveHuffmanTreeToFile(getLeftTree(tree), array);
    helper_saveHuffmanTreeToFile(getRightTree(tree), array);
}

void saveHuffmanTreeToFile(Tree *tree, FILE *fp)
{
    assert(tree);
    int allNodesCount = getNodesCountTree(tree);
    int leafNodesCount = getLeafNodesCountTree(tree);
    int bitmapSize = allNodesCount + leafNodesCount * 8; // (allNodesCount - leafNodesCount) * 1 + leafNodesCount * 9
    ArrayByte *array = createArrayByte(bitmapSize);
    helper_saveHuffmanTreeToFile(tree, array);
    fwrite(getContentArrayByte(array), getBytesLengthArrayByte(array), sizeof(unsigned char), fp);
    freeArrayByte(array);
}
