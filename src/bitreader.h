#pragma once

#include <stdio.h>

typedef struct bitreader BitReader;

BitReader *createBitReader(FILE *fp);

unsigned char readBitBitReader(BitReader *br);

unsigned char readByteBitReader(BitReader *br);

int hasNextByteBitReader(BitReader *br);

void freeBitReader(BitReader *br);

int getReadedBitsBitReader(BitReader *br);