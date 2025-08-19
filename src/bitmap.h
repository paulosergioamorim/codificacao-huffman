/**
 * @file bitmap.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para o TAD de Bitmap.
 */

#pragma once

#include <stdio.h>

/**
 * @typedef Bitmap
 * @brief Tipo opaco que representa a estrutura do bitmap.
 */
typedef struct bitmap Bitmap;

/**
 * @brief Cria um bitmap com capacidade fixa de bytes.
 * @param maxCapacity Capacidade máxima em bytes.
 * @return Ponteiro para o bitmap criado.
 */
Bitmap *createStaticBitmap(unsigned int maxCapacity);

/**
 * @brief Insere 1 bit na posição mais significativa do próximo byte.
 * @param bitmap Bitmap de destino.
 * @param bit Bit a ser inserido (0 ou 1).
 */
void insertMSBBitmap(Bitmap *bitmap, unsigned char bit);

/**
 * @brief Insere 1 bit na posição menos significativa do próximo byte.
 * @param bitmap Bitmap de destino.
 * @param bit Bit a ser inserido (0 ou 1).
 */
void insertLSBBitmap(Bitmap *bitmap, unsigned char bit);

/**
 * @brief Insere os 8 bits de um byte no bitmap, um por um (MSB primeiro).
 * @param bitmap Bitmap de destino.
 * @param byte Byte cujos bits serão inseridos.
 */
void insertByteBitmap(Bitmap *bitmap, unsigned char byte);

/**
 * @brief Insere um byte completo diretamente no buffer.
 * @param bitmap Bitmap de destino.
 * @param byte Byte a ser inserido.
 * @details Não verifica alinhamento e pode sobrescrever bits existentes.
 */
void insertAlignedByteBitmap(Bitmap *bitmap, unsigned char byte);

/**
 * @brief Retorna o byte na posição indicada.
 * @param bitmap Bitmap de origem.
 * @param index Índice do byte desejado.
 * @return Byte no índice fornecido.
 */
unsigned char getByteBitmap(Bitmap *bitmap, unsigned int index);

/**
 * @brief Retorna ponteiro para o vetor de bytes interno.
 * @param bitmap Bitmap de origem.
 * @return Ponteiro para os dados armazenados.
 */
unsigned char *getContentBitmap(Bitmap *bitmap);

/**
 * @brief Retorna a quantidade total de bits armazenados.
 * @param bitmap Bitmap de origem.
 * @return Número de bits válidos.
 */
unsigned int getBitsLengthBitmap(Bitmap *bitmap);

/**
 * @brief Retorna a quantidade de bytes utilizados atualmente.
 * @param bitmap Bitmap de origem.
 * @return Número de bytes ocupados.
 */
unsigned int getBytesLengthBitmap(Bitmap *bitmap);

/**
 * @brief Verifica se o bitmap está cheio.
 * @param bitmap Bitmap a ser verificado.
 * @return 1 se cheio, 0 caso contrário.
 */
int isFullBitmap(Bitmap *bitmap);

/**
 * @brief Limpa o conteúdo do bitmap, reinicializando os dados.
 * @param bitmap Bitmap a ser limpo.
 */
void clearBitmap(Bitmap *bitmap);

/**
 * @brief Libera a memória alocada pelo bitmap.
 * @param bitmap Bitmap a ser liberado.
 */
void freeBitmap(Bitmap *bitmap);

/**
 * @brief Escreve o conteúdo do bitmap em um arquivo.
 * @param bitmap Bitmap a ser escrito.
 * @param fp Ponteiro para arquivo de destino.
 */
void writeBitmap(Bitmap *bitmap, FILE *fp);
