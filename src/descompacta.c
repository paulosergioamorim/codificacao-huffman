#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "huffman.h"
#include "bitreader.h"

typedef Tree *(*table_fn)(Tree *);

static table_fn table[2] = {getLeftTree, getRightTree};

int consumeBit(BitReader *br, FILE *fp, Tree *huffmanTree, Tree **tree, unsigned char lastValidBits);

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
    unsigned char lastValidBits = 0;

    if (!fread(&lastValidBits, sizeof(lastValidBits), 1, inputFile) || !lastValidBits)
    {
        freeBitReader(br);
        fclose(inputFile);
        fclose(outputFile);
        free(outputFileName);
        return 0;
    }

    Tree *huffmanTree = createHuffmanTreeFromFile(br, &lastValidBits);
    Tree *cur = huffmanTree;

    while ((lastValidBits = consumeBit(br, outputFile, huffmanTree, &cur, lastValidBits)))
        ;

    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);
    free(outputFileName);

    return 0;
}

int consumeBit(BitReader *br, FILE *fp, Tree *huffmanTree, Tree **tree, unsigned char lastValidBits)
{
    unsigned char bit = readBitBitReader(br);
    Tree *next = table[bit](*tree);

    if (!hasNextByteBitReader(br))
        lastValidBits--;

    if (isLeafTree(next))
    {
        fputc(getValueTree(next), fp);
        *tree = huffmanTree;

        return lastValidBits;
    }

    *tree = next;

    return lastValidBits;
}

Tree *helper_createHuffmanTreeFromFile(BitReader *br, unsigned char *lastValidBits)
{
    int isLeafNode = readBitBitReader(br);

    if (isLeafNode)
    {
        unsigned char value = readByteBitReader(br);

        if (!hasNextByteBitReader(br))
            *lastValidBits -= getReadedBitsBitReader(br);

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
