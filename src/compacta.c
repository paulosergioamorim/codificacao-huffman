#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "huffman.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "rb");
    assert(inputFile);
    unsigned long freqs[UCHAR_MAX + 1] = {0};
    unsigned char c = 0;

    while (fread(&c, sizeof(c), 1, inputFile))
        freqs[c]++;

    int charsCount = 0;

    for (int i = 0; i <= UCHAR_MAX; i++)
        if (freqs[i])
            charsCount++;

    Heap *heap = createHeap(charsCount);

    for (int i = 0; i <= UCHAR_MAX; i++)
        if (freqs[i])
        {
            Tree *tree = createTree(i, freqs[i]);
            pushHeap(heap, tree);
        }

    char outputFileName[strlen(argv[1]) + sizeof(".comp")];
    snprintf(outputFileName, strlen(argv[1]) + sizeof(".comp"), "%s.comp", argv[1]);
    FILE *outputFile = fopen(outputFileName, "wb");
    assert(outputFile);

    Tree *huffmanTree = convertToHuffmanTree(heap);

    if (!huffmanTree)
    {
        unsigned char value = 0;
        fwrite(&value, 1, sizeof(value), outputFile);
        fclose(inputFile);
        fclose(outputFile);
        return 0;
    } // caso: arquivo vazio

    long totalBytes = ftell(inputFile);
    int serializedHuffmanTreeSize = getSerializedHuffmanTreeSize(huffmanTree);
    double expectedHeight = getExpectedHeightHuffmanTree(totalBytes, huffmanTree);

    if (!expectedHeight)
        expectedHeight = 1; // caso: árvore de huffman sendo a raiz folha

    BitArray **table = convertHuffmanTreeToTable(huffmanTree);
    BitArray *contentArrayByte = createStaticBitArray(serializedHuffmanTreeSize + totalBytes * expectedHeight + 0.1);

    rewind(inputFile);
    serializeHuffmanTree(huffmanTree, contentArrayByte);
    freeTree(huffmanTree);

    while (fread(&c, sizeof(c), 1, inputFile))
    {
        BitArray *array = table[c];
        unsigned int length = getBitsLengthBitArray(array);

        if (!length)
            insertLSBBitArray(contentArrayByte, 0); // caso: árvore de huffman sendo a raiz folha

        for (unsigned int i = 0; i < length; i++)
            insertLSBBitArray(contentArrayByte, getBitArray(array, i));
    }

    freeEncodingTable(table);
    fclose(inputFile);

    unsigned char lastValidBits = getBitsLengthBitArray(contentArrayByte) % 8;

    if (!lastValidBits)
        lastValidBits = 8; // caso: todos os bits do último byte são válidos

    unsigned char *encodedContent = getContentBitArray(contentArrayByte);
    unsigned int bytesLength = getBytesLengthBitArray(contentArrayByte);

    fwrite(&lastValidBits, sizeof(lastValidBits), 1, outputFile);
    fwrite(encodedContent, sizeof(unsigned char), bytesLength, outputFile);

    fclose(outputFile);
    freeBitArray(contentArrayByte);

    return 0;
}
