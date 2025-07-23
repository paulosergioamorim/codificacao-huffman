#pragma once

typedef struct bitarray BitArray;

BitArray *createStaticBitArray(int maxCapacity);

void insertMSBBitArray(BitArray *array, unsigned char bit);

void insertLSBBitArray(BitArray *array, unsigned char bit);

void insertByteBitArray(BitArray *array, unsigned char byte);

unsigned char getByteBitArray(BitArray *array, int index);

unsigned char getBitArray(BitArray *array, int index);

unsigned char *getContentBitArray(BitArray *array);

int getBitsLengthBitArray(BitArray *array);

int getBytesLengthBitArray(BitArray *array);

void freeBitArray(BitArray *array);