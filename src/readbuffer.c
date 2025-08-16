/**
 * @file readbuffer.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 */

#include "readbuffer.h"
#include <stdlib.h>
#include <string.h>

struct readbuffer
{
    FILE *fp;
    unsigned char *vec;
    unsigned int maxCapacity;
    unsigned int byteIndex;
    unsigned int bytesCount;
    int bitsLeft;
    int endOfFile;
};

void bufferFetch(ReadBuffer *buffer);

ReadBuffer *bufferInit(FILE *fp, unsigned int maxCapacity)
{
    ReadBuffer *buffer = malloc(sizeof(ReadBuffer));
    buffer->fp = fp;
    buffer->maxCapacity = maxCapacity;
    buffer->vec = calloc(buffer->maxCapacity, sizeof(unsigned char));
    buffer->bitsLeft = buffer->bytesCount = buffer->endOfFile = buffer->byteIndex = 0;

    return buffer;
}

unsigned char bufferNextBit(ReadBuffer *buffer)
{
    if (buffer->bitsLeft == 0)
    {
        buffer->bitsLeft = 8;
        buffer->byteIndex++;

        if (buffer->byteIndex == buffer->bytesCount)
            bufferFetch(buffer); // caso: consumiu o buffer inteiro
    } // caso: consumiu um byte

    unsigned char bit = buffer->vec[buffer->byteIndex];
    bit >>= 7;
    bit &= 0x01;
    buffer->vec[buffer->byteIndex] <<= 1;
    buffer->bitsLeft--;
    return bit;
}

unsigned char bufferNextByte(ReadBuffer *buffer)
{
    unsigned char byte = 0;

    for (int i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= bufferNextBit(buffer);
    }

    return byte;
}

unsigned char bufferNextAlignedByte(ReadBuffer *buffer)
{
    unsigned char byte = buffer->vec[buffer->byteIndex];
    buffer->byteIndex++;

    if (buffer->byteIndex == buffer->bytesCount)
        bufferFetch(buffer);

    return byte;
}

int bufferHasNextByte(ReadBuffer *buffer)
{
    if (buffer->byteIndex == buffer->bytesCount)
        bufferFetch(buffer);

    return !buffer->endOfFile || buffer->byteIndex < buffer->bytesCount;
}

int bufferIsLastByte(ReadBuffer *buffer)
{
    if (buffer->byteIndex == buffer->bytesCount)
        bufferFetch(buffer);

    if (!buffer->bytesCount)
        return 0;

    return buffer->endOfFile && buffer->byteIndex == buffer->bytesCount - 1;
}

void bufferFree(ReadBuffer *buffer)
{
    free(buffer->vec);
    free(buffer);
}

void bufferReset(ReadBuffer *buffer)
{
    fseek(buffer->fp, 0, SEEK_SET);
    buffer->bitsLeft = buffer->bytesCount = buffer->endOfFile = buffer->byteIndex = 0;
    memset(buffer->vec, 0, buffer->maxCapacity);
}

int bufferGetBitsLeft(ReadBuffer *buffer)
{
    return buffer->bitsLeft;
}

void bufferFetch(ReadBuffer *buffer)
{
    buffer->bytesCount = fread(buffer->vec, sizeof(unsigned char), buffer->maxCapacity, buffer->fp);
    buffer->endOfFile = buffer->bytesCount < buffer->maxCapacity;
    buffer->byteIndex = 0;
    buffer->bitsLeft = 8;
}
