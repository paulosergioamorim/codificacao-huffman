/**
 * @file descompacta.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Programa Descompactador
 */

#include "bitmap.h"
#include "huffman.h"
#include "readbuffer.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "rb");

    if (!inputFile)
    {
        fprintf(stderr, "Falha ao abrir arquivo de entrada.\n");
        return 1;
    }

    char *outputFileName = removeExtentionFromString(argv[1]);
    FILE *outputFile = fopen(outputFileName, "wb");
    free(outputFileName);

    if (!outputFileName)
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

    unsigned char lastValidBits = bufferNextAlignedByte(buffer);
    Bitmap *bitmap = createStaticBitmap(BUFFER_SIZE);
    Tree *huffmanTree = createHuffmanTreeFromFile(buffer);

    if (bufferIsLastByte(buffer))
        lastValidBits -= (8 - bufferGetBitsLeft(buffer)); // caso: árvore serializada consumiu bits do último byte

    Tree *cur = huffmanTree;

    while (lastValidBits)
    {
        cur = consumeBit(buffer, bitmap, huffmanTree, cur);

        if (bufferIsLastByte(buffer))
            lastValidBits--; // caso: consimiu um bit do último byte

        if (!lastValidBits || isFullBitmap(bitmap))
        {
            writeBitmap(bitmap, outputFile);
            clearBitmap(bitmap);
        } // caso: escrever um bloco inteiro ou o último bloco
    }

    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);
    freeBitmap(bitmap);
    bufferFree(buffer);

    return 0;
}
