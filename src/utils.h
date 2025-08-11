/*
 * UFES - Universidade Federal do Espírito Santo
 * Autor: Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * Projeto: Compactador e Descompactador
 */

#pragma once

#include <limits.h>

#define ASCII_SIZE UCHAR_MAX + 1

char *addExtentionToString(const char *str);

void removeExtentionFromString(const char *str);
