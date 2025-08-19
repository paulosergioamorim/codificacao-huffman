/**
 * @file utils.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para funções utilitárias do projeto Compactador e Descompactador.
 * @details Este arquivo define macros e funções auxiliares.
 */

#pragma once

#include <limits.h>

/**
 * @def ASCII_SIZE
 * @brief Total de caracteres possíveis em 8 bits (0 a 255).
 */
#define ASCII_SIZE UCHAR_MAX + 1

/**
 * @def BUFFER_SIZE
 * @brief Tamanho do buffer interno usado para leitura e escrita de arquivos (1MB).
 */
#define BUFFER_SIZE 1024 * 1024

/**
 * @brief Retorna uma nova string com a extensão ".comp" adicionada.
 * @param str String original.
 * @return Nova string alocada dinamicamente com a extensão adicionada.
 */
char *addExtentionToString(const char *str);

/**
 * @brief Retorna uma nova string sem a extensão do nome de arquivo.
 * @param str String original.
 * @return Nova string alocada dinamicamente sem a extensão.
 */
char *removeExtentionFromString(const char *str);
