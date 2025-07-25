#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "bitarray.h"
#include <stdint.h>

struct bitarray
{
    unsigned char *vec;
    int capacity;
    int size;
};

BitArray *createStaticBitArray(int capacity)
{
    BitArray *array = malloc(sizeof(BitArray));
    assert(array);
    array->capacity = ++capacity;
    array->size = 0;
    array->vec = calloc((array->capacity + 7) / 8, sizeof(unsigned char));
    assert(array->vec);

    return array;
}

void insertMSBBitArray(BitArray *array, unsigned char bit)
{
    assert(array);
    if (array->size == array->capacity)
    {
        fprintf(stderr, "Overflow in bit array in %dth bit.\n", array->size);
        exit(EXIT_FAILURE);
    }

    bit &= 0x80;
    int indexOfByte = array->size / 8;
    int indexOfBit = array->size % 8;
    bit >>= indexOfBit;
    array->vec[indexOfByte] |= bit;
    array->size++;
}

void insertLSBBitArray(BitArray *array, unsigned char bit)
{
    assert(array);
    if (array->size == array->capacity)
    {
        fprintf(stderr, "Overflow in bit array in %dth bit.\n", array->size);
        exit(EXIT_FAILURE);
    }

    bit &= 0x01;
    int indexOfByte = array->size / 8;
    int indexOfBit = array->size % 8;
    int shift = 7 - indexOfBit;

    bit <<= shift;
    array->vec[indexOfByte] |= bit;
    array->size++;
}

void insertByteBitArray(BitArray *array, unsigned char byte)
{
    assert(array);
    for (int i = 0; i < 8; i++)
        insertMSBBitArray(array, byte << i);
}

unsigned char *getContentBitArray(BitArray *array)
{
    assert(array);
    return array->vec;
}

int getBitsLengthBitArray(BitArray *array)
{
    assert(array);
    return array->size;
}

int getBytesLengthBitArray(BitArray *array)
{
    assert(array);
    return (array->size + 7) / 8;
}

void freeBitArray(BitArray *array)
{
    assert(array);
    assert(array->vec);
    free(array->vec);
    free(array);
}

unsigned char getByteBitArray(BitArray *array, int index)
{
    assert(array);
    assert(array->vec);
    if (index >= (array->size + 7) / 8)
        return 0;

    return array->vec[index];
}

unsigned char getBitArray(BitArray *array, int index)
{
    assert(array);
    assert(array->vec);
    if (index >= array->size)
        return 0;

    int indexToByte = index / 8;
    int indexToBit = index % 8;

    unsigned char byte = array->vec[indexToByte];
    int shift = 7 - indexToBit;

    return byte >> shift & 0x01;
}
