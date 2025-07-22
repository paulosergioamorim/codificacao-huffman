#define ENCODER_PROGRAM

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "huffman.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        exit(1);
    }

    FILE *inputFile = fopen(argv[1], "r");
    assert(inputFile);
    unsigned int freqs[UCHAR_MAX] = {0};
    unsigned char c = 0;

    while (fscanf(inputFile, "%c", &c) != EOF)
        freqs[c]++;

    int charsCount = 0;

    for (int i = 0; i < UCHAR_MAX; i++)
        if (freqs[i])
            charsCount++;

    Heap *heap = createHeap(charsCount);

    for (int i = 0; i < UCHAR_MAX; i++)
        if (freqs[i])
        {
            Tree *tree = createTree(i, freqs[i]);
            pushHeap(heap, tree);
        }

    Tree *huffmanTree = convertToHuffmanTree(heap);
    ArrayByte **table = convertHuffmanTreeToTable(huffmanTree);

    FILE *outputFile = fopen(argv[2], "wb+");
    assert(outputFile);

    ArrayByte *contentArrayByte = createArrayByte(ftell(inputFile) * 8);
    rewind(inputFile);

    saveHuffmanTreeToFile(huffmanTree, outputFile);
    freeTree(huffmanTree);

    while (fscanf(inputFile, "%c", &c) != EOF)
    {
        ArrayByte *array = table[c];
        int length = getBitsLengthArrayByte(array);
        for (int i = 0; i < length; i++)
            insertLSBArrayByte(contentArrayByte, getBitArrayByte(array, i));
    }

    freeEncodingTable(table);
    fclose(inputFile);

    unsigned int bitsLength = getBitsLengthArrayByte(contentArrayByte);

    fwrite(&bitsLength, 1, sizeof(unsigned int), outputFile);
    fwrite(getContentArrayByte(contentArrayByte), getBytesLengthArrayByte(contentArrayByte), sizeof(unsigned char), outputFile);

    fclose(outputFile);
    freeArrayByte(contentArrayByte);

    return 0;
}
