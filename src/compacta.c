#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include "huffman.h"

#define BUFFER_SIZE 1024 * 8

void write_buffer(BitArray *array, FILE *fp);

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
        fclose(inputFile);
        fclose(outputFile);
        return 0;
    } // caso: arquivo vazio

    long totalBytes = ftell(inputFile);
    int serializedHuffmanTreeSize = getSerializedHuffmanTreeSize(huffmanTree);

    BitArray *array;

    if (isLeafTree(huffmanTree))
    {
        array = createStaticBitArray(serializedHuffmanTreeSize + totalBytes);
        serializeHuffmanTree(huffmanTree, array);
        freeTree(huffmanTree);

        for (long i = 0; i < totalBytes; i++)
            insertLSBBitArray(array, 0);

        unsigned char lastValidBits = getBitsLengthBitArray(array) % 8;

        if (!lastValidBits)
            lastValidBits = 8; // caso: todos os bits do último byte são válidos

        unsigned char *encodedContent = getContentBitArray(array);
        unsigned int bytesLength = getBytesLengthBitArray(array);

        fwrite(encodedContent, sizeof(unsigned char), bytesLength, outputFile);
        fwrite(&lastValidBits, sizeof(lastValidBits), 1, outputFile);
        freeBitArray(array);
        fclose(inputFile);
        fclose(outputFile);
        return 0;
    } // caso: árvore de huffman sendo a raiz folha

    unsigned int *table = convertHuffmanTreeToTable(huffmanTree);

    fseek(inputFile, 0, SEEK_SET);

    array = createStaticBitArray(BUFFER_SIZE);
    serializeHuffmanTree(huffmanTree, array);
    freeTree(huffmanTree);

    while (fread(&c, sizeof(c), 1, inputFile))
    {
        unsigned int code = table[c];
        int len = log2(code);

        for (int i = len - 1; i >= 0; i--)
        {
            if (!isFullBitArray(array))
            {
                insertLSBBitArray(array, code >> i);
                continue;
            }

            write_buffer(array, outputFile);
            insertLSBBitArray(array, code >> i);
        }
    }

    freeEncodingTable(table);
    fclose(inputFile);

    unsigned char lastValidBits = getBitsLengthBitArray(array) % 8;

    if (!lastValidBits)
        lastValidBits = 8; // caso: todos os bits do último byte são válidos

    write_buffer(array, outputFile);
    fwrite(&lastValidBits, sizeof(lastValidBits), 1, outputFile);
    fclose(outputFile);
    freeBitArray(array);

    return 0;
}

void write_buffer(BitArray *array, FILE *fp)
{
    unsigned int len = getBytesLengthBitArray(array);
    unsigned char *content = getContentBitArray(array);
    fwrite(content, sizeof(unsigned char), len, fp);
    clearBitArray(array);
}
