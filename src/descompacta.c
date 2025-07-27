#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "huffman.h"
#include "bitreader.h"

#define BUFFER_SIZE 1024

typedef Tree *(*table_fn)(Tree *);

static table_fn table[2] = {getLeftTree, getRightTree};

int consumeBit(BitReader *br, unsigned char *buffer, int *bufferCount, Tree *huffmanTree, Tree **tree, unsigned char lastValidBits);

Tree *createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits);

Tree *helper_createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits);

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

    BitReader *br = createBitReader(inputFile);
    fseek(inputFile, -1, SEEK_END);
    unsigned char lastValidBits = 0;
    unsigned char buffer[BUFFER_SIZE] = {0};
    int bufferCount = 0;

    if (!fread(&lastValidBits, sizeof(lastValidBits), 1, inputFile))
    {
        freeBitReader(br);
        fclose(inputFile);
        fclose(outputFile);
        free(outputFileName);
        return 0;
    } // caso: arquivo vazio

    fseek(inputFile, 0, SEEK_SET);
    Tree *huffmanTree = createHuffmanTreeFromFile(br, &lastValidBits);
    Tree *cur = huffmanTree;

    while ((lastValidBits = consumeBit(br, buffer, &bufferCount, huffmanTree, &cur, lastValidBits)))
        if (bufferCount == BUFFER_SIZE)
        {
            fwrite(&buffer, sizeof(unsigned char), bufferCount, outputFile);
            memset(buffer, 0, BUFFER_SIZE);
            bufferCount = 0;
        }

    fwrite(&buffer, sizeof(unsigned char), bufferCount, outputFile);
    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);
    free(outputFileName);

    return 0;
}

int consumeBit(BitReader *br, unsigned char *buffer, int *bufferCount, Tree *huffmanTree, Tree **tree, unsigned char lastValidBits)
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
    buffer[*bufferCount] = value;
    *bufferCount += 1;
    *tree = huffmanTree;

    return lastValidBits;
}

Tree *helper_createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits)
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

    setLeftTree(tree, helper_createHuffmanTreeFromFile(br, lastValidBits));
    setRightTree(tree, helper_createHuffmanTreeFromFile(br, lastValidBits));

    return tree;
}

Tree *createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits)
{
    return helper_createHuffmanTreeFromFile(br, lastValidBits);
}
