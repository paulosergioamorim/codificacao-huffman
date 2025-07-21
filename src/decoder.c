#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "arraybyte.h"
#include "tree.h"
#include "bitreader.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Insira um arquivo para descomprimir.\n");
        exit(1);
    }

    FILE *INPUT_FILE = fopen(argv[1], "rb");
    FILE *OUTPUT_FILE = fopen(argv[2], "w+");
    assert(INPUT_FILE);
    assert(OUTPUT_FILE);
    Tree *huffmanTree = createHuffmanTreeFromFile(INPUT_FILE);
    Tree *cur = huffmanTree;
    int validBits = 0;
    fread(&validBits, 1, sizeof(int), INPUT_FILE);
    BitReader *br = createBitReader(INPUT_FILE);

    for (int i = 0; i < validBits; i++)
    {
        byte bit = readBitBitReader(br);

        if (!bit)
            cur = getLeftTree(cur);
        else
            cur = getRightTree(cur);

        if (isLeafTree(cur))
        {
            fputc(getValueTree(cur), OUTPUT_FILE);
            cur = huffmanTree;
        }
    }

    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(INPUT_FILE);
    fclose(OUTPUT_FILE);

    return 0;
}
