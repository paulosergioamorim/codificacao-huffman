/*
 * UFES - Universidade Federal do Espírito Santo
 * Autor: Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * Projeto: Compactador e Descompactador
 */

#pragma once

#define BUFFER_SIZE 1024 * 1024
#include <stdio.h>

typedef struct readbuffer ReadBuffer;

ReadBuffer *bufferInit(FILE *fp);

unsigned char bufferNextBit(ReadBuffer *buffer);

unsigned char bufferNextByte(ReadBuffer *buffer);

unsigned char bufferNextAlignedByte(ReadBuffer *buffer);

int bufferIsLastByte(ReadBuffer *buffer);

int bufferHasNextByte(ReadBuffer *buffer);

void bufferReset(ReadBuffer *buffer);

void bufferFree(ReadBuffer *buffer);

int bufferGetBitIndex(ReadBuffer *buffer);