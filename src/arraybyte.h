#pragma once

typedef struct arraybyte ArrayByte;

ArrayByte *createArrayByte(int maxCapacity);

void insertMSBArrayByte(ArrayByte *array, unsigned char bit);

void insertLSBArrayByte(ArrayByte *array, unsigned char bit);

void insertByteArrayByte(ArrayByte *array, unsigned char byte);

unsigned char getByteArrayByte(ArrayByte *array, int index);

unsigned char getBitArrayByte(ArrayByte *array, int index);

unsigned char *getContentArrayByte(ArrayByte *array);

int getBitsLengthArrayByte(ArrayByte *array);

int getBytesLengthArrayByte(ArrayByte *array);

void freeArrayByte(ArrayByte *array);