#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "heap.h"
#include "arraybyte.h"

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Insira um arquivo para comprimir.\n");
        exit(1);
    }

    FILE *INPUT_FILE = fopen(argv[1], "rb");
    assert(INPUT_FILE);
    unsigned int freqs[UCHAR_MAX] = {0};
    unsigned char c = 0;

    while (fscanf(INPUT_FILE, "%c", &c) != EOF)
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

    FILE *OUTPUT_FILE = fopen(argv[2], "wb+");
    assert(OUTPUT_FILE);

    ArrayByte *contentArrayByte = createArrayByte(ftell(INPUT_FILE) * 8);
    rewind(INPUT_FILE);

    saveHuffmanTreeToFile(huffmanTree, OUTPUT_FILE);

    while (fscanf(INPUT_FILE, "%c", &c) != EOF)
    {
        ArrayByte *array = table[c];
        int length = getBitsLengthArrayByte(array);
        for (int i = 0; i < length; i++)
            insertLSBArrayByte(contentArrayByte, getBitArrayByte(array, i));
    }

    unsigned int bitsLength = getBitsLengthArrayByte(contentArrayByte);

    fwrite(&bitsLength, 1, sizeof(int), OUTPUT_FILE);
    fwrite(getContentArrayByte(contentArrayByte), getBytesLengthArrayByte(contentArrayByte), sizeof(byte), OUTPUT_FILE);

    freeEncodingTable(table);
    freeTree(huffmanTree);
    fclose(INPUT_FILE);
    fclose(OUTPUT_FILE);
    freeArrayByte(contentArrayByte);

    return 0;
}
