#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define EXTENSION_SIZE sizeof(".comp")

char *addExtentionToString(const char *str)
{
    int len = strlen(str);
    char *newStr = malloc((len + EXTENSION_SIZE) * sizeof(char));
    snprintf(newStr, len + EXTENSION_SIZE, "%s.comp", str);
    return newStr;
}

void removeExtentionFromString(const char *str)
{
    char *lastDot = strrchr(str, '.');
    *lastDot = 0;
}

FILE *openFileToRead(const char *path)
{
    FILE *fp = fopen(path, "rb");

    if (!fp)
    {
        fprintf(stderr, "[ERRO] Falha ao abrir arquivo.\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}

FILE *openFileToWrite(const char *path)
{
    FILE *fp = fopen(path, "wb");

    if (!fp)
    {
        fprintf(stderr, "[ERRO] Falha ao abrir arquivo.\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}