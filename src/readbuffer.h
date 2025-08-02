#pragma once

#define BUFFER_SIZE 1024 * 1024
#include <stdio.h>
#include <stdint.h>

typedef struct readbuffer ReadBuffer;

ReadBuffer *bufferInit(FILE *fp);

uint8_t bufferNextBit(ReadBuffer *buffer);

uint8_t bufferNextByte(ReadBuffer *buffer);

uint8_t bufferNextAlignedByte(ReadBuffer *buffer);

int bufferIsLastByte(ReadBuffer *buffer);

int bufferHasNextByte(ReadBuffer *buffer);

void bufferReset(ReadBuffer *buffer);

void bufferFree(ReadBuffer *buffer);

int bufferGetBitIndex(ReadBuffer *buffer);