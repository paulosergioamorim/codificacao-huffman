#define DECODER_PROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "huffman.h"
#include "bitreader.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "rb");
    FILE *outputFile = fopen(argv[2], "w+");
    assert(inputFile);
    assert(outputFile);

    Tree *huffmanTree = createHuffmanTreeFromFile(inputFile);
    Tree *cur = huffmanTree;

    unsigned int validBits = 0;
    fread(&validBits, 1, sizeof(unsigned int), inputFile);

    BitReader *br = createBitReader(inputFile);

    for (int i = 0; i < validBits; i++)
    {
        unsigned char bit = readBitBitReader(br);

        if (isLeafTree(cur))
        {
            fputc(getValueTree(cur), outputFile);
            cur = huffmanTree;
            continue;
        }

        if (bit)
        {
            cur = getRightTree(cur);
            continue;
        }

        cur = getLeftTree(cur);
    }

    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
