/**
 * @file readbuffer.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para o TAD de Buffer de Leitura.
 */

#pragma once

#include <stdio.h>

/**
 * @typedef ReadBuffer
 * @brief Tipo opaco que encapsula o estado do buffer de leitura.
 */
typedef struct readbuffer ReadBuffer;

/**
 * @brief Inicializa um novo buffer de leitura com capacidade fixa.
 * @param fp Ponteiro para o arquivo aberto em modo binário de leitura.
 * @param maxCapacity Capacidade máxima do buffer em bytes.
 * @return Ponteiro para o buffer alocado.
 */
ReadBuffer *bufferInit(FILE *fp, unsigned int maxCapacity);

/**
 * @brief Lê o próximo bit do fluxo de dados.
 * @param buffer Buffer de leitura.
 * @return Bit lido (0 ou 1).
 */
unsigned char bufferNextBit(ReadBuffer *buffer);

/**
 * @brief Lê o próximo byte desalinhado.
 * @param buffer Buffer de leitura.
 * @return Byte lido (pode cruzar fronteiras de byte).
 */
unsigned char bufferNextByte(ReadBuffer *buffer);

/**
 * @brief Lê o próximo byte alinhado.
 * @param buffer Buffer de leitura.
 * @return Byte lido.
 * @details Pode causar perda de dados se o buffer estiver desalinhado.
 */
unsigned char bufferNextAlignedByte(ReadBuffer *buffer);

/**
 * @brief Verifica se o byte atual é o último do arquivo.
 * @param buffer Buffer de leitura.
 * @return 1 se for o último byte, 0 caso contrário.
 */
int bufferIsLastByte(ReadBuffer *buffer);

/**
 * @brief Verifica se há mais bytes disponíveis para leitura.
 *
 * @param buffer Buffer de leitura.
 * @return 1 se houver bytes restantes, 0 se for fim de arquivo.
 */
int bufferHasNextByte(ReadBuffer *buffer);

/**
 * @brief Reinicia o buffer e reposiciona o arquivo para o início.
 *
 * @param buffer Buffer de leitura.
 */
void bufferReset(ReadBuffer *buffer);

/**
 * @brief Libera a memória associada ao buffer.
 * @param buffer Buffer a ser liberado.
 * @note O ponteiro de arquivo não é fechado por esta função.
 */
void bufferFree(ReadBuffer *buffer);

/**
 * @brief Retorna quantos bits restam para ler no byte atual.
 * @param buffer Buffer de leitura.
 * @return Valor de 1 a 8.
 */
int bufferGetBitsLeft(ReadBuffer *buffer);
