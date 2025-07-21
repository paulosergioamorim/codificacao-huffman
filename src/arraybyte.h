#pragma once

#include "utils.h"

typedef struct arraybyte ArrayByte;

ArrayByte *createArrayByte(int maxCapacity);

void insertMSBArrayByte(ArrayByte *array, byte bit);

void insertLSBArrayByte(ArrayByte *array, byte bit);

void insertByteArrayByte(ArrayByte *array, byte byte);

byte getByteArrayByte(ArrayByte *array, int index);

byte getBitArrayByte(ArrayByte *array, int index);

byte *getContentArrayByte(ArrayByte *array);

int getBitsLengthArrayByte(ArrayByte *array);

int getBytesLengthArrayByte(ArrayByte *array);

void freeArrayByte(ArrayByte *array);