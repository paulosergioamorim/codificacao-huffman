#include "queue.h"
#include "bitmap.h"
#include <string.h>
#include <stdio.h>

int CountCharStr(char *str, char c);

int main(int argc, char const *argv[])
{
    Queue *queue = CreateQueue();
    char *str = "bom esse bombom";

    for (int i = 0; i < strlen(str); i++)
    {
        char c = str[i];
        int frequency = CountCharStr(str, c);

        if (!ExistsQueue(queue, c))
            Enqueue(queue, CreateTree(c, frequency, NULL, NULL));
    }

    Tree *huffmanTree = ToHuffmanTree(queue);
    FILE *pFile = fopen("compact.bin", "wb+");
    bitmap *huffmanTreeBm = EncodeHuffmanTree(huffmanTree);

    bitmap *contentBm = bitmapInit(strlen(str) * GetHeightTree(huffmanTree));

    if (!pFile)
    {
        return 0;
    }

    for (int i = 0; i < strlen(str); i++)
    {
        bitmap *charBm = EncodeCharHuffmanTree(huffmanTree, str[i]);

        for (int i = 0; i < bitmapGetLength(charBm); i++)
        {
            bitmapAppendLeastSignificantBit(contentBm, bitmapGetBit(charBm, i));
        }

        bitmapLibera(charBm);
    }

    fwrite(bitmapGetContents(huffmanTreeBm), sizeof(char), (bitmapGetLength(huffmanTreeBm) + 7) / 8, pFile);
    // fwrite(bitmapGetContents(contentBm), sizeof(char), (bitmapGetLength(contentBm) + 7) / 8, pFile);
    bitmapLibera(huffmanTreeBm);
    bitmapLibera(contentBm);
    fclose(pFile);

    FreeTree(huffmanTree);

    return 0;
}

int CountCharStr(char *str, char c)
{
    int count = 0;

    while (*str)
    {
        if (*str == c)
            count++;

        str++;
    }

    return count;
}
