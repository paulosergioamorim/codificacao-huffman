#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include "huffman.h"
#include "readbuffer.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        exit(1);
    }

    char outputFileName[strlen(argv[1]) + sizeof(".comp")];
    snprintf(outputFileName, strlen(argv[1]) + sizeof(".comp"), "%s.comp", argv[1]);

    FILE *inputFile = fopen(argv[1], "rb");
    FILE *outputFile = fopen(outputFileName, "wb");
    ReadBuffer *buffer = bufferInit(inputFile);

    if (!bufferHasNextByte(buffer))
    {
        fclose(inputFile);
        fclose(outputFile);
        bufferFree(buffer);
        return 0;
    } // caso: arquivo vazio

    unsigned long freqs[UCHAR_MAX + 1] = {0};

    while (bufferHasNextByte(buffer))
    {
        uint8_t byte = bufferNextAlignedByte(buffer);
        freqs[byte]++;
    }

    int bytesCount = 0;

    for (int i = 0; i <= UCHAR_MAX; i++)
        if (freqs[i])
            bytesCount++;

    Heap *heap = createHeap(bytesCount);

    for (int i = 0; i <= UCHAR_MAX; i++)
        if (freqs[i])
        {
            Tree *tree = createTree(i, freqs[i]);
            pushHeap(heap, tree);
        }

    Tree *huffmanTree = convertToHuffmanTree(heap);
    unsigned int *table = convertHuffmanTreeToTable(huffmanTree);
    BitArray *array = createStaticBitArray(BUFFER_SIZE * 8);
    uint8_t lastValidBits = 8;

    fwrite(&lastValidBits, sizeof(uint8_t), 1, outputFile); // caso: suponha que todos os últimos bits são válidos
    serializeHuffmanTree(huffmanTree, array);
    freeTree(huffmanTree);
    bufferReset(buffer);

    while (bufferHasNextByte(buffer))
    {
        uint8_t byte = bufferNextAlignedByte(buffer);

        unsigned int code = table[byte];
        int len = log2(code);

        if (code == 1 && isFullBitArray(array))
        {
            writeBitArray(array, outputFile);
            clearBitArray(array);
        }

        if (code == 1)
        {
            insertLSBBitArray(array, 0);
            continue;
        }

        for (int i = len - 1; i >= 0; i--)
        {
            if (isFullBitArray(array))
            {
                writeBitArray(array, outputFile);
                clearBitArray(array);
            }

            insertLSBBitArray(array, code >> i);
        }
    }

    writeBitArray(array, outputFile);

    lastValidBits = getBitsLengthBitArray(array) % 8;

    if (lastValidBits > 0)
    {
        fseek(outputFile, 0, SEEK_SET);
        fwrite(&lastValidBits, sizeof(uint8_t), 1, outputFile);
    } // caso: menos que 8 bits válidos

    freeEncodingTable(table);
    fclose(inputFile);
    fclose(outputFile);
    freeBitArray(array);
    bufferFree(buffer);

    return 0;
}
