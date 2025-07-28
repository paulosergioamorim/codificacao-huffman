#pragma once

#include <stdio.h>

typedef struct bitarray BitArray;

BitArray *createStaticBitArray(unsigned int maxCapacity);

void insertMSBBitArray(BitArray *array, unsigned char bit);

void insertLSBBitArray(BitArray *array, unsigned char bit);

void insertByteBitArray(BitArray *array, unsigned char byte);

unsigned char getByteBitArray(BitArray *array, unsigned int index);

unsigned char getBitArray(BitArray *array, unsigned int index);

unsigned char *getContentBitArray(BitArray *array);

unsigned int getBitsLengthBitArray(BitArray *array);

unsigned int getBytesLengthBitArray(BitArray *array);

int isFullBitArray(BitArray *array);

void clearBitArray(BitArray *array);

void freeBitArray(BitArray *array);

void writeBitArray(BitArray *array, FILE *fp);