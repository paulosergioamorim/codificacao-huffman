#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "arraybyte.h"

struct arraybyte
{
    byte *vec;
    int capacity;
    int size;
};

void grow(ArrayByte *array);

ArrayByte *createArrayByte(int capacity)
{
    ArrayByte *array = malloc(sizeof(ArrayByte));
    assert(array);
    array->capacity = capacity;
    array->size = 0;
    array->vec = calloc(array->capacity, sizeof(byte));
    assert(array->vec);

    return array;
}

void insertMSBArrayByte(ArrayByte *array, byte bit)
{
    assert(array->size / 8 < array->capacity);
    bit &= 0x80;
    int indexOfByte = array->size / 8;
    int indexOfBit = array->size % 8;
    bit >>= indexOfBit;
    array->vec[indexOfByte] |= bit;
    array->size++;
}

void insertLSBArrayByte(ArrayByte *array, byte bit)
{
    assert(array->size / 8 < array->capacity);
    bit &= 0x01;
    int indexOfByte = array->size / 8;
    int indexOfBit = array->size % 8;
    int shift = 7 - indexOfBit;

    bit <<= shift;
    array->vec[indexOfByte] |= bit;
    array->size++;
}

void insertByteArrayByte(ArrayByte *array, byte byte)
{
    for (int i = 0; i < 8; i++)
        insertMSBArrayByte(array, byte << i);
}

void insertCompactIntArrayByte(ArrayByte *array, int value)
{
    for (int i = 0; i < log2(value); i++)
        insertLSBArrayByte(array, (byte)((value >> i) & 0x01));
}

byte *getContentArrayByte(ArrayByte *array)
{
    return array->vec;
}

int getBitsLengthArrayByte(ArrayByte *array)
{
    return array->size;
}

int getBytesLengthArrayByte(ArrayByte *array)
{
    return (array->size + 7) / 8;
}

void freeArrayByte(ArrayByte *array)
{
    free(array->vec);
    free(array);
}

byte getByteArrayByte(ArrayByte *array, int index)
{
    if ((int)(array->size / 8.0 + 1) <= index)
        return 0;

    return array->vec[index];
}

byte getBitArrayByte(ArrayByte *array, int index)
{
    if (index >= array->size)
        return 0;

    int indexToByte = index / 8;
    int indexToBit = index % 8;

    byte byte = array->vec[indexToByte];
    int shift = 7 - indexToBit;

    return byte >> shift & 0x01;
}
