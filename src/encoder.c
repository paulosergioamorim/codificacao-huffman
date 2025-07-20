#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "heap.h"
#include "arraybyte.h"

void freeEncodingTable(ArrayByte **table);

int main(int argc, char const *argv[])
{
    if (argc < 3)
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
    ArrayByte **table = convertHuffmanTreeToTable(huffmanTree);

    FILE *fpOut = fopen(argv[2], "wb+");
    assert(fpOut);

    ArrayByte *contentArrayByte = createArrayByte(ftell(fpIn));
    rewind(fpIn);

    saveHuffmanTreeToFile(huffmanTree, fpOut);

    while (fscanf(fpIn, "%c", &c) != EOF)
    {
        ArrayByte *array = table[c];
        int length = getBitsLengthArrayByte(array);
        for (int i = 0; i < length; i++)
            insertLSBArrayByte(contentArrayByte, getBitArrayByte(array, i));
    }

    unsigned int bitsLength = getBitsLengthArrayByte(contentArrayByte);

    fwrite(&bitsLength, 1, sizeof(int), fpOut);
    fwrite(getContentArrayByte(contentArrayByte), getBytesLengthArrayByte(contentArrayByte), sizeof(byte), fpOut);

    freeEncodingTable(table);
    freeTree(huffmanTree);
    fclose(fpIn);
    fclose(fpOut);
    freeArrayByte(contentArrayByte);

    return 0;
}
