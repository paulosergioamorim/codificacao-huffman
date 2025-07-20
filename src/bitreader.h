#pragma once

#include <stdio.h>
#include "utils.h"

typedef struct bitreader BitReader;

BitReader *createBitReader(FILE *fp);

byte readBitBitReader(BitReader *br);

byte readByteBitReader(BitReader *br);

void freeBitReader(BitReader *br);