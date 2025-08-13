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
    int byteIndex;
    int bytesCount;
    int bitIndex;
    int endOfFile;
};

void bufferFetch(ReadBuffer *buffer);

ReadBuffer *bufferInit(FILE *fp)
{
    ReadBuffer *buffer = malloc(sizeof(ReadBuffer));
    buffer->fp = fp;
    buffer->vec = calloc(BUFFER_SIZE, sizeof(unsigned char));
    buffer->bitIndex = buffer->bytesCount = buffer->endOfFile = buffer->byteIndex = 0;

    return buffer;
}

unsigned char bufferNextBit(ReadBuffer *buffer)
{
    if (buffer->bitIndex == 0)
    {
        buffer->bitIndex = 8;
        buffer->byteIndex++;

        if (buffer->byteIndex == buffer->bytesCount)
            bufferFetch(buffer);
    }

    unsigned char bit = buffer->vec[buffer->byteIndex];
    bit >>= 7;
    bit &= 0x01;
    buffer->vec[buffer->byteIndex] <<= 1;
    buffer->bitIndex--;
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

    if (!buffer->bytesCount)
        return 0;

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
    buffer->bitIndex = buffer->bytesCount = buffer->endOfFile = buffer->byteIndex = 0;
    memset(buffer->vec, 0, BUFFER_SIZE);
}

int bufferGetBitIndex(ReadBuffer *buffer)
{
    return buffer->bitIndex;
}

void bufferFetch(ReadBuffer *buffer)
{
    buffer->bytesCount = fread(buffer->vec, sizeof(unsigned char), BUFFER_SIZE, buffer->fp);
    buffer->endOfFile = buffer->bytesCount < BUFFER_SIZE;
    buffer->byteIndex = 0;
    buffer->bitIndex = 8;
}
