/*
 * UFES - Universidade Federal do Espírito Santo
 * Autor: Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * Projeto: Compactador e Descompactador
 */

#pragma once

#include <stdio.h>

typedef struct bitmap Bitmap;

Bitmap *createStaticBitmap(unsigned int maxCapacity);

void insertMSBBitmap(Bitmap *bitmap, unsigned char bit);

void insertLSBBitmap(Bitmap *bitmap, unsigned char bit);

void insertByteBitmap(Bitmap *bitmap, unsigned char byte);

void insertAlignedByteBitmap(Bitmap *bitmap, unsigned char byte);

unsigned char getByteBitmap(Bitmap *bitmap, unsigned int index);

unsigned char getBitmap(Bitmap *bitmap, unsigned int index);

unsigned char *getContentBitmap(Bitmap *bitmap);

unsigned int getBitsLengthBitmap(Bitmap *bitmap);

unsigned int getBytesLengthBitmap(Bitmap *bitmap);

int isFullBitmap(Bitmap *bitmap);

void clearBitmap(Bitmap *bitmap);

void freeBitmap(Bitmap *bitmap);

void writeBitmap(Bitmap *bitmap, FILE *fp);