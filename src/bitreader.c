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
    br->buffer = 0;
    br->fp = fp;
    br->bitsCount = 0;

    return br;
}

unsigned char readBitBitReader(BitReader *br)
{
    if (br->bitsCount == 0)
    {
        br->bitsCount = 8;
        br->buffer = fgetc(br->fp);
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