#pragma once

#include <stdio.h>

typedef struct bitreader BitReader;

BitReader *createBitReader(FILE *fp);

void clearBufferBitReader(BitReader *br);

unsigned char readBitBitReader(BitReader *br);

unsigned char readByteBitReader(BitReader *br);

int hasNextByteBitReader(BitReader *br);

void freeBitReader(BitReader *br);