/**
 * @file readbuffer.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para o TAD de Buffer de Leitura.
 * @details Este módulo implementa um buffer de leitura de alta performance,
 * projetado para minimizar chamadas de I/O ao ler arquivos grandes. Ele lê
 * pedaços (chunks) do arquivo para a memória e fornece funções para consumir
 * dados tanto bit a bit quanto byte a byte.
 */

#pragma once

#include <stdio.h>

/**
 * @def BUFFER_SIZE
 * @brief Define o tamanho do buffer interno de memória em bytes.
 * @details Um buffer maior resulta em menos chamadas de leitura ao disco (`fread`),
 * melhorando a performance, ao custo de um maior uso de memória RAM.
 * O valor atual é 1MB.
 */
#define BUFFER_SIZE 1024 * 1024

/**
 * @typedef ReadBuffer
 * @brief Tipo opaco que encapsula o estado do buffer de leitura.
 */
typedef struct readbuffer ReadBuffer;

/**
 * @brief Cria e inicializa um novo buffer de leitura.
 * @param fp O ponteiro de arquivo (`FILE*`) para o arquivo a ser lido.
 * O arquivo deve ser aberto em modo de leitura binária (ex: "rb").
 * @return Um ponteiro para a estrutura `ReadBuffer` recém-alocada.
 */
ReadBuffer *bufferInit(FILE *fp);

/**
 * @brief Lê o próximo bit do stream de dados.
 * @param buffer O ponteiro para o buffer de leitura.
 * @return O valor do bit lido (0 ou 1).
 */
unsigned char bufferNextBit(ReadBuffer *buffer);

/**
 * @brief Lê o próximo byte do stream de dados, compondo-o bit a bit.
 * @details Esta função chama `bufferNextBit` 8 vezes para construir um byte.
 * É a maneira correta de ler dados de conteúdo (decodificados), pois respeita
 * o fluxo de bits, que pode não estar alinhado com os bytes originais do arquivo.
 * @param buffer O ponteiro para o buffer de leitura.
 * @return O byte lido.
 */
unsigned char bufferNextByte(ReadBuffer *buffer);

/**
 * @brief Lê o próximo byte completo diretamente do buffer.
 * @details Esta função é mais rápida que `bufferNextByte`, pois lê o byte
 * diretamente da memória. Deve ser usada apenas para ler dados que se sabe
 * estarem alinhados por byte, como o cabeçalho do arquivo.
 * @param buffer O ponteiro para o buffer de leitura.
 * @return O byte lido.
 */
unsigned char bufferNextAlignedByte(ReadBuffer *buffer);

/**
 * @brief Verifica se o byte atual é o último byte do arquivo.
 * @param buffer O ponteiro para o buffer de leitura.
 * @return 1 (verdadeiro) se o cursor está no último byte do arquivo, 0 caso contrário.
 */
int bufferIsLastByte(ReadBuffer *buffer);

/**
 * @brief Verifica se ainda há bytes a serem lidos no arquivo.
 * @param buffer O ponteiro para o buffer de leitura.
 * @return 1 (verdadeiro) se houver pelo menos mais um byte para ler, 0 se o fim
 * do arquivo foi alcançado.
 */
int bufferHasNextByte(ReadBuffer *buffer);

/**
 * @brief Reinicia o estado do buffer e reposiciona o ponteiro do arquivo para o início.
 * @param buffer O ponteiro para o buffer de leitura.
 */
void bufferReset(ReadBuffer *buffer);

/**
 * @brief Libera toda a memória associada ao buffer de leitura.
 * @note Esta função **não** fecha o ponteiro de arquivo (`FILE*`) que foi
 * passado para `bufferInit`. O chamador é responsável por fechar o arquivo.
 * @param buffer O ponteiro para o buffer a ser liberado.
 */
void bufferFree(ReadBuffer *buffer);

/**
 * @brief Retorna o índice do bit atual dentro do byte corrente.
 * @details O valor varia de 8 (início de um byte) a 1. Pode ser usado para
 * verificar se o leitor está em uma fronteira de byte.
 * @param buffer O ponteiro para o buffer de leitura.
 * @return O índice do bit (8 a 1).
 */
int bufferGetBitIndex(ReadBuffer *buffer);