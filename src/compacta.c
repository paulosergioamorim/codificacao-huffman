/**
 * @file compacta.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Programa Compactador
 */

#include "heap.h"
#include "huffman.h"
#include "readbuffer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
#include <math.h>
#endif

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");

    if (!inputFile)
    {
        fprintf(stderr, "Falha ao abrir arquivo de entrada.\n");
        return 1;
    }

    char *outputFileName = addExtentionToString(argv[1]);
    FILE *outputFile = fopen(outputFileName, "wb");
    free(outputFileName);

    if (!outputFile)
    {
        fprintf(stderr, "Falha ao abrir arquivo de saída.\n");
        fclose(inputFile);
        return 1;
    }

    ReadBuffer *buffer = bufferInit(inputFile, BUFFER_SIZE);

    if (!bufferHasNextByte(buffer))
    {
        fclose(inputFile);
        fclose(outputFile);
        bufferFree(buffer);
        return 0;
    } // caso: arquivo vazio

    unsigned long freqs[ASCII_SIZE] = {0};

    while (bufferHasNextByte(buffer))
    {
        unsigned char byte = bufferNextAlignedByte(buffer);
        freqs[byte]++;
    }

    int bytesCount = 0;

    for (int i = 0; i < ASCII_SIZE; i++)
        if (freqs[i])
            bytesCount++;

    Heap *heap = createHeap(bytesCount);

    for (int i = 0; i < ASCII_SIZE; i++)
        if (freqs[i])
        {
            Tree *tree = createTree(i, freqs[i]);
            pushHeap(heap, tree);
        }

    Tree *huffmanTree = convertHeapToHuffmanTree(heap);
    unsigned int *table = convertHuffmanTreeToTable(huffmanTree);
    Bitmap *bitmap = createStaticBitmap(BUFFER_SIZE);
    serializeHuffmanTree(huffmanTree, bitmap);
    freeTree(huffmanTree);

    unsigned char lastValidBits = 8;
    fwrite(&lastValidBits, sizeof(unsigned char), 1,
           outputFile); // caso: suponha que todos os últimos bits são válidos
    bufferReset(buffer);

    while (bufferHasNextByte(buffer))
    {
        unsigned char byte = bufferNextAlignedByte(buffer);
        unsigned int code = table[byte];

        if (code == 1 && isFullBitmap(bitmap))
        {
            writeBitmap(bitmap, outputFile);
            clearBitmap(bitmap);
        } // caso: arquivo com 1 valor de byte e bitmap cheio

        if (code == 1)
        {
            insertLSBBitmap(bitmap, 0);
            continue;
        } // caso: arquivo com 1 valor de byte

#ifdef __GNUC__
        int len = 31 - __builtin_clz(code); // compila diretamente para instrução de máquina (count leading zeros)
#else
        int len = log2(code);
#endif

        for (int i = len - 1; i >= 0; i--)
        {
            if (isFullBitmap(bitmap))
            {
                writeBitmap(bitmap, outputFile);
                clearBitmap(bitmap);
            } // caso: bitmap cheio

            insertLSBBitmap(bitmap, code >> i);
        }
    }

    writeBitmap(bitmap, outputFile);
    lastValidBits = getBitsLengthBitmap(bitmap) % 8;

    if (lastValidBits > 0)
    {
        fseek(outputFile, 0, SEEK_SET);
        fwrite(&lastValidBits, sizeof(unsigned char), 1, outputFile);
    } // caso: menos que 8 bits válidos

    free(table);
    fclose(inputFile);
    fclose(outputFile);
    freeBitmap(bitmap);
    bufferFree(buffer);

    return 0;
}
