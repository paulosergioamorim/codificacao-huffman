#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "readbuffer.h"
#include "huffman.h"
#include "bitarray.h"
#include "utils.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        exit(1);
    }

    FILE *inputFile = openFileToRead(argv[1]);
    removeExtentionFromString(argv[1]);
    FILE *outputFile = openFileToWrite(argv[1]);
    ReadBuffer *buffer = bufferInit(inputFile);

    if (!bufferHasNextByte(buffer))
    {
        fclose(inputFile);
        fclose(outputFile);
        bufferFree(buffer);
        return 0;
    } // caso: arquivo vazio

    uint8_t lastValidBits = bufferNextAlignedByte(buffer);
    BitArray *array = createStaticBitArray(BUFFER_SIZE * 8);
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
