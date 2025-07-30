#include <stdlib.h>
#include <string.h>
#include "readbuffer.h"

struct readbuffer
{
    FILE *fp;
    uint8_t *vec;
    int byteIndex;
    int bytesCount;
    int bitsCount;
    int endOfFile;
};

int buffer_fetch(ReadBuffer *buffer);

int buffer_fetch(ReadBuffer *buffer)
{
    int bytesReaded = fread(buffer->vec, sizeof(uint8_t), BUFFER_SIZE, buffer->fp);
    buffer->bytesCount = bytesReaded;
    buffer->byteIndex = 0;
    buffer->bitsCount = 8;

    if (bytesReaded < BUFFER_SIZE)
        buffer->endOfFile = 1;

    return bytesReaded;
}

ReadBuffer *bufferInit(FILE *fp)
{
    ReadBuffer *buffer = malloc(sizeof(ReadBuffer));
    buffer->fp = fp;
    buffer->vec = calloc(BUFFER_SIZE, sizeof(uint8_t));
    buffer->bitsCount = buffer->bytesCount = buffer->endOfFile = buffer->byteIndex = 0;

    return buffer;
}

uint8_t bufferNextBit(ReadBuffer *buffer)
{
    if (buffer->bitsCount == 0)
    {
        buffer->bitsCount = 8;
        buffer->byteIndex++;

        if (buffer->byteIndex == buffer->bytesCount)
            buffer->bytesCount = buffer_fetch(buffer);
    }

    uint8_t bit = buffer->vec[buffer->byteIndex];
    bit >>= 7;
    bit &= 0x01;
    buffer->vec[buffer->byteIndex] <<= 1;
    buffer->bitsCount--;
    return bit;
}

uint8_t bufferNextByte(ReadBuffer *buffer)
{
    uint8_t byte = 0;

    for (int i = 0; i < 8; i++)
    {
        byte <<= 1;
        byte |= bufferNextBit(buffer);
    }

    return byte;
}

uint8_t bufferNextAlignedByte(ReadBuffer *buffer)
{
    buffer->bitsCount = 8;
    uint8_t byte = buffer->vec[buffer->byteIndex];
    buffer->byteIndex++;

    if (buffer->byteIndex == buffer->bytesCount)
        buffer->bytesCount = buffer_fetch(buffer);
    return byte;
}

int bufferHasNextByte(ReadBuffer *buffer)
{
    if (buffer->byteIndex == buffer->bytesCount)
        buffer->bytesCount = buffer_fetch(buffer);

    if (!buffer->bytesCount)
        return 0;

    return !buffer->endOfFile || buffer->byteIndex < buffer->bytesCount;
}

int bufferIsLastByte(ReadBuffer *buffer)
{
    if (buffer->byteIndex == buffer->bytesCount)
        buffer->bytesCount = buffer_fetch(buffer);

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
    buffer->byteIndex = 0;
    buffer->bytesCount = 0;
    buffer->bitsCount = 0;
    buffer->endOfFile = 0;
    memset(buffer->vec, 0, BUFFER_SIZE);
}

int bufferGetBitsCount(ReadBuffer *buffer)
{
    return buffer->bitsCount;
}