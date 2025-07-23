#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "huffman.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "r");
    assert(inputFile);
    unsigned int freqs[UCHAR_MAX] = {0};
    unsigned char c = 0;

    while (fscanf(inputFile, "%c", &c) != EOF)
        freqs[c]++;

    int charsCount = 0;

    for (int i = 0; i < UCHAR_MAX; i++)
        if (freqs[i])
            charsCount++;

    Heap *heap = createHeap(charsCount);

    for (int i = 0; i < UCHAR_MAX; i++)
        if (freqs[i])
        {
            Tree *tree = createTree(i, freqs[i]);
            pushHeap(heap, tree);
        }

    Tree *huffmanTree = convertToHuffmanTree(heap);
    BitArray **table = convertHuffmanTreeToTable(huffmanTree);

    FILE *outputFile = fopen(argv[2], "wb");
    assert(outputFile);

    int serializedHuffmanTreeSize = getNodesCountTree(huffmanTree) + 8 * (getLeafNodesCountTree(huffmanTree));
    float expectedHeight = getExpectedHeightHuffmanTree(ftell(inputFile), huffmanTree);
    BitArray *contentArrayByte = createStaticBitArray(serializedHuffmanTreeSize + ftell(inputFile) * expectedHeight);
    rewind(inputFile);

    serializeHuffmanTree(huffmanTree, contentArrayByte);
    freeTree(huffmanTree);

    while (fscanf(inputFile, "%c", &c) != EOF)
    {
        BitArray *array = table[c];
        int length = getBitsLengthBitArray(array);

        for (int i = 0; i < length; i++)
            insertLSBBitArray(contentArrayByte, getBitArray(array, i));
    }

    freeEncodingTable(table);
    fclose(inputFile);

    unsigned char lastValidBits = getBitsLengthBitArray(contentArrayByte) % 8;
    unsigned char *encodedContent = getContentBitArray(contentArrayByte);
    int bytesLength = getBytesLengthBitArray(contentArrayByte);

    fwrite(&lastValidBits, 1, sizeof(lastValidBits), outputFile);
    fwrite(encodedContent, bytesLength, sizeof(unsigned char), outputFile);

    fclose(outputFile);
    freeBitArray(contentArrayByte);

    return 0;
}
