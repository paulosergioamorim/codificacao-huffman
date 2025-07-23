#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "bitreader.h"

typedef Tree *(*table_fn)(Tree *);

static table_fn table[2] = {getLeftTree, getRightTree};

Tree *consumeBit(BitReader *br, FILE *fp, Tree *huffmanTree, Tree *tree);

Tree *createHuffmanTreeFromFile(BitReader *br);

Tree *helper_createHuffmanTreeFromFile(BitReader *br);

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "rb");
    FILE *outputFile = fopen(argv[2], "wb");
    assert(inputFile);
    assert(outputFile);

    BitReader *br = createBitReader(inputFile);
    unsigned char lastValidBits = readByteBitReader(br);
    Tree *huffmanTree = createHuffmanTreeFromFile(br);
    Tree *cur = huffmanTree;

    while (hasNextByteBitReader(br))
        cur = consumeBit(br, outputFile, huffmanTree, cur);

    for (unsigned char i = 0; i < lastValidBits; i++)
        cur = consumeBit(br, outputFile, huffmanTree, cur);

    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

Tree *consumeBit(BitReader *br, FILE *fp, Tree *huffmanTree, Tree *tree)
{
    unsigned char bit = readBitBitReader(br);
    Tree *next = table[bit](tree);

    if (isLeafTree(next))
    {
        fputc(getValueTree(next), fp);
        return huffmanTree;
    }

    return next;
}

Tree *helper_createHuffmanTreeFromFile(BitReader *br)
{
    int isLeafNode = readBitBitReader(br);

    if (isLeafNode)
    {
        unsigned char value = readByteBitReader(br);
        return createTree(value, 0);
    }

    Tree *tree = createTree(0, 0);

    setLeftTree(tree, helper_createHuffmanTreeFromFile(br));
    setRightTree(tree, helper_createHuffmanTreeFromFile(br));

    return tree;
}

Tree *createHuffmanTreeFromFile(BitReader *br)
{
    return helper_createHuffmanTreeFromFile(br);
}
