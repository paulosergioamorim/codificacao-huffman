#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "huffman.h"
#include "bitreader.h"
#include "bitarray.h"

#define BUFFER_SIZE 1024 * 8

typedef Tree *(*table_fn)(Tree *);

static table_fn table[2] = {getLeftTree, getRightTree};

int consumeBit(BitReader *br, BitArray *array, Tree *huffmanTree, Tree **tree, unsigned char lastValidBits);

Tree *createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        exit(1);
    }

    char *outputFileName = strdup(argv[1]);
    char *lastDot = strrchr(outputFileName, '.');
    *lastDot = 0;
    FILE *inputFile = fopen(argv[1], "rb");
    FILE *outputFile = fopen(outputFileName, "wb");
    assert(inputFile);
    assert(outputFile);

    fseek(inputFile, -1, SEEK_END);
    unsigned char lastValidBits = 0;

    if (!fread(&lastValidBits, sizeof(lastValidBits), 1, inputFile))
    {
        fclose(inputFile);
        fclose(outputFile);
        free(outputFileName);
        return 0;
    } // caso: arquivo vazio

    BitReader *br = createBitReader(inputFile);
    BitArray *array = createStaticBitArray(BUFFER_SIZE);
    fseek(inputFile, 0, SEEK_SET);
    Tree *huffmanTree = createHuffmanTreeFromFile(br, &lastValidBits);
    Tree *cur = huffmanTree;

    while ((lastValidBits = consumeBit(br, array, huffmanTree, &cur, lastValidBits)))
        if (isFullBitArray(array))
        {
            writeBitArray(array, outputFile);
            clearBitArray(array);
        }

    writeBitArray(array, outputFile);
    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);
    free(outputFileName);
    freeBitArray(array);

    return 0;
}

int consumeBit(BitReader *br, BitArray *array, Tree *huffmanTree, Tree **tree, unsigned char lastValidBits)
{
    unsigned char bit = readBitBitReader(br);
    Tree *next = *tree;

    if (!hasNextByteBitReader(br))
        lastValidBits--;

    if (!isLeafTree(huffmanTree))
        next = table[bit](next); // caso: raiz da árvore de huffman não é uma folha

    if (!isLeafTree(next))
    {
        *tree = next;
        return lastValidBits;
    }

    unsigned char value = getValueTree(next);
    insertByteBitArray(array, value);
    *tree = huffmanTree;

    return lastValidBits;
}

Tree *createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits)
{
    int isLeafNode = readBitBitReader(br);

    if (isLeafNode)
    {
        unsigned char value = readByteBitReader(br);

        if (!hasNextByteBitReader(br))
            *lastValidBits -= getReadedBitsBitReader(br); // caso: a árvore de huffman ocupou o último byte

        return createTree(value, 0);
    }

    Tree *tree = createTree(0, 0);

    setLeftTree(tree, createHuffmanTreeFromFile(br, lastValidBits));
    setRightTree(tree, createHuffmanTreeFromFile(br, lastValidBits));

    return tree;
}
