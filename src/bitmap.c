/**
 * @file bitmap.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 */

#include "bitmap.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct bitmap
{
    unsigned char *vec;
    unsigned int capacity;
    unsigned int size;
};

Bitmap *createStaticBitmap(unsigned int capacity)
{
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    assert(bitmap);
    bitmap->capacity = capacity;
    bitmap->size = 0;
    bitmap->vec = calloc(bitmap->capacity, sizeof(unsigned char));
    assert(bitmap->vec);

    return bitmap;
}

void insertMSBBitmap(Bitmap *bitmap, unsigned char bit)
{
    if (bitmap->size == bitmap->capacity * 8)
    {
        fprintf(stderr, "[ERROR] Overflow in bit bitmap.\n");
        exit(EXIT_FAILURE);
    }

    bit &= 0x80; // máscara 0b10000000
    unsigned int indexOfByte = bitmap->size / 8;
    int indexOfBit = bitmap->size % 8;
    bit >>= indexOfBit;              // posiciona o bit
    bitmap->vec[indexOfByte] |= bit; // adiciona o bit ao byte
    bitmap->size++;
}

void insertLSBBitmap(Bitmap *bitmap, unsigned char bit)
{
    if (bitmap->size == bitmap->capacity * 8)
    {
        fprintf(stderr, "[ERROR] Overflow in bit bitmap.\n");
        exit(EXIT_FAILURE);
    }

    bit &= 0x01; // máscara 0b00000001
    unsigned int indexOfByte = bitmap->size / 8;
    int indexOfBit = bitmap->size % 8;
    int shift = 7 - indexOfBit;

    bit <<= shift;                   // posiciona o bit
    bitmap->vec[indexOfByte] |= bit; // adiciona o bit ao byte
    bitmap->size++;
}

void insertByteBitmap(Bitmap *bitmap, unsigned char byte)
{
    for (int i = 0; i < 8; i++)
        insertMSBBitmap(bitmap, byte << i);
}

void insertAlignedByteBitmap(Bitmap *bitmap, unsigned char byte)
{
    if (bitmap->size + 7 == bitmap->capacity * 8)
    {
        fprintf(stderr, "[ERROR] Overflow in bit bitmap.\n");
        exit(EXIT_FAILURE);
    }

    unsigned int indexOfByte = bitmap->size / 8;
    bitmap->vec[indexOfByte] = byte;
    bitmap->size += 8;
}

unsigned char *getContentBitmap(Bitmap *bitmap)
{
    return bitmap->vec;
}

unsigned int getBitsLengthBitmap(Bitmap *bitmap)
{
    return bitmap->size;
}

unsigned int getBytesLengthBitmap(Bitmap *bitmap)
{
    return (bitmap->size + 7) / 8;
}

int isFullBitmap(Bitmap *bitmap)
{
    return bitmap->size == bitmap->capacity * 8;
}

void clearBitmap(Bitmap *bitmap)
{
    memset(bitmap->vec, 0, bitmap->capacity);
    bitmap->size = 0;
}

void freeBitmap(Bitmap *bitmap)
{
    free(bitmap->vec);
    free(bitmap);
}

unsigned char getByteBitmap(Bitmap *bitmap, unsigned int index)
{
    if (index >= (bitmap->size + 7) / 8)
        return 0;

    return bitmap->vec[index];
}

void writeBitmap(Bitmap *bitmap, FILE *fp)
{
    unsigned int len = getBytesLengthBitmap(bitmap);
    unsigned char *content = getContentBitmap(bitmap);
    fwrite(content, sizeof(unsigned char), len, fp);
}