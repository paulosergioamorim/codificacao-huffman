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

    FILE *in = fopen(argv[1], "rb");
    FILE *out = fopen(argv[2], "w+");
    assert(in);
    assert(out);
    Tree *huffmanTree = createHuffmanTreeFromFile(in);
    Tree *cur = huffmanTree;
    int validBits = 0;
    fread(&validBits, 1, sizeof(int), in);
    BitReader *br = createBitReader(in);

    for (int i = 0; i < validBits; i++)
    {
        byte bit = readBitBitReader(br);

        if (!bit)
            cur = getLeftTree(cur);
        else
            cur = getRightTree(cur);

        if (isLeafTree(cur))
        {
            fputc(getValueTree(cur), out);
            cur = huffmanTree;
        }
    }

    freeBitReader(br);
    freeTree(huffmanTree);
    fclose(in);
    fclose(out);

    return 0;
}
