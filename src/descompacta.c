#include "bitarray.h"
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

    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    if (!(inputFile = fopen(argv[1], "rb")))
    {
        fprintf(stderr, "Falha ao abrir arquivo de entrada.\n");
        return 1;
    }

    removeExtentionFromString(argv[1]);

    if (!(outputFile = fopen(argv[1], "wb")))
    {
        fprintf(stderr, "Falha ao abrir arquivo de saída.\n");
        fclose(inputFile);
        return 1;
    }

    ReadBuffer *buffer = bufferInit(inputFile);

    if (!bufferHasNextByte(buffer))
    {
        fclose(inputFile);
        fclose(outputFile);
        bufferFree(buffer);
        return 0;
    } // caso: arquivo vazio

    unsigned char lastValidBits = bufferNextAlignedByte(buffer);
    BitArray *array = createStaticBitArray(BUFFER_SIZE);
    Tree *huffmanTree = createHuffmanTreeFromFile(buffer);

    if (bufferIsLastByte(buffer))
        lastValidBits -= (8 - bufferGetBitIndex(buffer)); // caso: árvore serializada consumiu bits do último byte

    Tree *cur = huffmanTree;

    while (lastValidBits)
    {
        consumeBit(buffer, array, huffmanTree, &cur);

        if (bufferIsLastByte(buffer))
            lastValidBits--; // caso: consimiu um bit do último byte

        if (!lastValidBits || isFullBitArray(array))
        {
            writeBitArray(array, outputFile);
            clearBitArray(array);
        } // caso: escrever um bloco inteiro ou o último bloco
    }

    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);
    freeBitArray(array);
    bufferFree(buffer);

    return 0;
}
