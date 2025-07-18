#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "heap.h"
#include "bitmap.h"

void freeEncodingTable(unsigned char **table);

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        exit(1);
    }

    Heap *heap = createHeap();
    FILE *fpIn = fopen(argv[1], "rb");
    assert(fpIn);
    int freqs[UCHAR_MAX] = {0};
    unsigned char c = 0;

    while (fscanf(fpIn, "%c", &c) != EOF)
        freqs[c]++;

    for (int i = 0; i < UCHAR_MAX; i++)
        if (freqs[i])
        {
            Tree *tree = createTree(i, freqs[i]);
            pushHeap(heap, tree);
        }

    Tree *huffmanTree = convertToHuffmanTree(heap);
    int huffmanTreeHeight = getHeightTree(huffmanTree);

    unsigned char **table = convertHuffmanTreeToTable(huffmanTree);

    FILE *fpOut = fopen("saida.txt", "wb+");
    assert(fpOut);

    bitmap *bm = bitmapInit(ftell(fpIn) * getHeightTree(huffmanTree));

    rewind(fpIn);

    while (fscanf(fpIn, "%c", &c) != EOF)
        for (int i = 0; table[c][i] != 2 && i < huffmanTreeHeight; i++)
            bitmapAppendLeastSignificantBit(bm, table[c][i]);

    unsigned int validBitsCount = bitmapGetLength(bm);

    fwrite(&validBitsCount, 1, sizeof(int), fpOut);
    fwrite(bitmapGetContents(bm), (int)ceil(validBitsCount / 8), sizeof(char), fpOut);

    freeEncodingTable(table);
    freeTree(huffmanTree);
    fclose(fpIn);
    fclose(fpOut);
    bitmapLibera(bm);

    return 0;
}

void freeEncodingTable(unsigned char **table)
{
    assert(table);

    for (int i = 0; i < UCHAR_MAX; i++)
        free(table[i]);

    free(table);
}
