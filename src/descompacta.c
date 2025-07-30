#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "readbuffer.h"
#include "huffman.h"
#include "bitarray.h"

typedef Tree *(*table_fn)(Tree *);

static table_fn table[2] = {getLeftTree, getRightTree};

void consumeBit(ReadBuffer *buffer, BitArray *array, Tree *huffmanTree, Tree **tree);

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "rb");
    char *lastDot = strrchr(argv[1], '.');
    *lastDot = 0;
    FILE *outputFile = fopen(argv[1], "wb");
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
        lastValidBits -= (8 - bufferGetBitsCount(buffer)); // caso: árvore serializada consumiu bits do último byte

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

void consumeBit(ReadBuffer *buffer, BitArray *array, Tree *huffmanTree, Tree **tree)
{
    unsigned char bit = bufferNextBit(buffer);
    Tree *next = *tree;

    if (!isLeafTree(huffmanTree))
        next = table[bit](next); // caso: raiz da árvore de huffman não é uma folha

    if (!isLeafTree(next))
    {
        *tree = next;
        return;
    }

    unsigned char value = getValueTree(next);
    insertAlignedByteBitArray(array, value);
    *tree = huffmanTree;
}

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer)
{
    uint8_t isLeafNode = bufferNextBit(buffer);

    if (isLeafNode)
    {
        uint8_t value = bufferNextByte(buffer);
        return createTree(value, 0);
    }

    Tree *tree = createTree(0, 0);

    setLeftTree(tree, createHuffmanTreeFromFile(buffer));
    setRightTree(tree, createHuffmanTreeFromFile(buffer));

    return tree;
}
