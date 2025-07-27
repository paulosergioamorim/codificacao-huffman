#include "bitreader.h"
#include <stdlib.h>

struct bitreader
{
    FILE *fp;
    unsigned char buffer;
    long pos;
    long totalBytes;
    int bitsCount;
};

BitReader *createBitReader(FILE *fp)
{
    BitReader *br = malloc(sizeof(BitReader));
    br->fp = fp;
    br->bitsCount = br->buffer = br->pos = 0;
    long cur_pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    br->totalBytes = ftell(fp);
    fseek(fp, cur_pos, SEEK_SET);

    return br;
}

unsigned char readBitBitReader(BitReader *br)
{
    if (br->bitsCount == 0)
    {
        br->bitsCount = 8;

        if (!fread(&br->buffer, sizeof(br->buffer), 1, br->fp))
            return 0;

        br->pos++;
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
    return br->pos != br->totalBytes - 1;
}

int getReadedBitsBitReader(BitReader *br)
{
    return 8 - br->bitsCount;
}
