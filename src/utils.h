/**
 * @file utils.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para funções utilitárias do projeto Compactador e Descompactador.
 * @details Este arquivo define macros e funções auxiliares
 */

#pragma once

#include <limits.h>

/**
 * @def ASCII_SIZE
 * @brief Define o número total de caracteres possíveis em uma tabela ASCII estendida (8 bits).
 * @details O valor é 256 (de 0 a 255), correspondendo a todos os valores que um
 * tipo `unsigned char` pode representar. Útil para criar tabelas de frequência de bytes.
 */
#define ASCII_SIZE UCHAR_MAX + 1

/**
 * @def BUFFER_SIZE
 * @brief Define o tamanho do buffer interno de memória em bytes.
 * @details Um buffer maior resulta em menos chamadas de leitura ao disco (`fread`),
 * melhorando a performance, ao custo de um maior uso de memória RAM.
 * O valor atual é 1MB. Esse buffer é maior do que o tamanho máximo que uma árvore de Huffman pode ocupar
 * (9 * 256 + 255) (importante para o bitmap de compactação).
 */
#define BUFFER_SIZE 1024 * 1024

/**
 * @brief Adiciona a extensão ".comp" a uma string.
 * @details Esta função recebe uma string, aloca memória para uma nova string
 * e retorna essa nova string com a extensão ".comp" concatenada ao final.
 *
 * @param str A string original à qual a extensão será adicionada.
 * @return Um ponteiro para uma **nova string alocada dinamicamente** contendo o resultado.
 * O chamador é responsável por liberar a memória alocada com `free()`.
 */
char *addExtentionToString(const char *str);

/**
 * @brief Remove a extensão de um nome de arquivo, retornando uma nova string.
 * @details A função cria uma cópia da string de entrada e a trunca no último
 * caractere '.', removendo a extensão. A string original não é modificada.
 *
 * @param str A string original da qual a extensão será removida.
 * @return Um ponteiro para uma **nova string alocada dinamicamente** sem a extensão.
 * O chamador é responsável por liberar a memória alocada com `free()`.
 */
char *removeExtentionFromString(const char *str);