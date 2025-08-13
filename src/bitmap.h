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
 * @brief Cria um bitmap com uma capacidade máxima fixa de bytes.
 * @details "Static" no nome refere-se à capacidade fixa, não ao tipo de alocação de memória.
 * @param maxCapacity A capacidade máxima do bitmap, em **bytes**.
 * @return Um ponteiro para a estrutura Bitmap recém-alocada.
 */
Bitmap *createStaticBitmap(unsigned int maxCapacity);

/**
 * @brief Insere o bit mais significativo no bitmap.
 * @param bitmap O bitmap a ser modificado.
 * @param bit O bit mais significativo a ser inserido.
 */
void insertMSBBitmap(Bitmap *bitmap, unsigned char bit);

/**
 * @brief Insere o bit menos significativo no bitmap.
 * @param bitmap O bitmap a ser modificado.
 * @param bit O bit menos significativo a ser inserido.
 */
void insertLSBBitmap(Bitmap *bitmap, unsigned char bit);

/**
 * @brief Insere os 8 bits de um byte no bitmap, um de cada vez.
 * @param bitmap O bitmap a ser modificado.
 * @param byte O byte cujos bits serão inseridos.
 */
void insertByteBitmap(Bitmap *bitmap, unsigned char byte);

/**
 * @brief Insere um byte completo diretamente no buffer de bytes do bitmap.
 * @warning Esta função tem um comportamento específico: ela **não** verifica se o
 * bitmap está alinhado a um byte. Ela escreve o byte na posição de byte atual (`size / 8`)
 * e avança o contador de bits em 8. Se `size` não for múltiplo de 8, isto
 * **sobrescreverá** bits já existentes no byte atual.
 * @param bitmap O bitmap a ser modificado.
 * @param byte O byte a ser inserido.
 */
void insertAlignedByteBitmap(Bitmap *bitmap, unsigned char byte);

/**
 * @brief Retorna um byte específico do conteúdo do bitmap.
 * @param bitmap O bitmap a ser consultado.
 * @param index O **índice do byte** a ser retornado.
 * @return O byte no índice especificado.
 */
unsigned char getByteBitmap(Bitmap *bitmap, unsigned int index);

/**
 * @brief Retorna um bit específico do conteúdo do bitmap.
 * @param bitmap O bitmap a ser consultado.
 * @param index O **índice do bit** a ser retornado.
 * @return O valor do bit (1 ou 0) no índice especificado.
 */
unsigned char getBitmap(Bitmap *bitmap, unsigned int index);

/**
 * @brief Retorna um ponteiro direto para o conteúdo de dados do bitmap.
 * @details Útil para operações de baixo nível ou para escrever o conteúdo em um arquivo.
 * @param bitmap O bitmap a ser consultado.
 * @return Um ponteiro `unsigned char*` para o vetor de dados.
 */
unsigned char *getContentBitmap(Bitmap *bitmap);

/**
 * @brief Retorna o número de bits atualmente armazenados no bitmap.
 * @param bitmap O bitmap a ser consultado.
 * @return O tamanho atual em **bits**.
 */
unsigned int getBitsLengthBitmap(Bitmap *bitmap);

/**
 * @brief Retorna o número de bytes necessários para armazenar os bits atuais.
 * @details Calcula o teto da divisão de `bits / 8`.
 * @param bitmap O bitmap a ser consultado.
 * @return O tamanho atual em **bytes**.
 */
unsigned int getBytesLengthBitmap(Bitmap *bitmap);

/**
 * @brief Verifica se o bitmap atingiu sua capacidade máxima.
 * @param bitmap O bitmap a ser consultado.
 * @return 1 (verdadeiro) se estiver cheio, 0 (falso) caso contrário.
 */
int isFullBitmap(Bitmap *bitmap);

/**
 * @brief Limpa o conteúdo do bitmap, zerando todos os dados e o contador de tamanho.
 * @details Permite reutilizar o bitmap sem a necessidade de desalocar e realocar memória.
 * @param bitmap O bitmap a ser limpo.
 */
void clearBitmap(Bitmap *bitmap);

/**
 * @brief Libera toda a memória associada ao bitmap.
 * @param bitmap O bitmap a ser liberado.
 */
void freeBitmap(Bitmap *bitmap);

/**
 * @brief Escreve o conteúdo do bitmap em um arquivo.
 * @param bitmap O bitmap cujo conteúdo será escrito.
 * @param fp O ponteiro de arquivo (`FILE*`) para o arquivo de saída.
 */
void writeBitmap(Bitmap *bitmap, FILE *fp);