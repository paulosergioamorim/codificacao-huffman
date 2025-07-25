#include "bitreader.h"
#include <stdlib.h>
#include <assert.h>

struct bitreader
{
    FILE *fp;
    unsigned char buffer;
    int bitsCount;
};

BitReader *createBitReader(FILE *fp)
{
    BitReader *br = malloc(sizeof(BitReader));
    br->fp = fp;
    br->bitsCount = 0;
    br->buffer = 0;

    return br;
}

unsigned char readBitBitReader(BitReader *br)
{
    if (br->bitsCount == 0)
    {
        br->bitsCount = 8;

        if (fread(&br->buffer, sizeof(br->buffer), 1, br->fp) == (size_t)EOF)
            printf("Fim do arquivo.\n");
    }

    unsigned char value = (br->buffer >> 7) & 0x01;
    br->buffer <<= 1;
    br->bitsCount--;

    return value;
}

unsigned char readByteBitReader(BitReader *br)
{
    unsigned char value = 0;

    for (int i = 0; i < 8; i++)
        value = (value << 1) | readBitBitReader(br);

    return value;
}

void freeBitReader(BitReader *br)
{
    free(br);
}

int hasNextByteBitReader(BitReader *br)
{
    int c1 = fgetc(br->fp);

    if (c1 == EOF)
        return 0;

    ungetc(c1, br->fp);
    return 1;
}

int getReadedBitsBitReader(BitReader *br)
{
    return 8 - br->bitsCount;
}