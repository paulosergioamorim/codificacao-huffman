#pragma once
#include <limits.h>
#include <stdio.h>

#define ASCII_SIZE UCHAR_MAX + 1

char *addExtentionToString(const char *str);

void removeExtentionFromString(const char *str);

FILE *openFileToRead(const char *path);

FILE *openFileToWrite(const char *path);
