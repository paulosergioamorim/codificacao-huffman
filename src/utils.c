#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
