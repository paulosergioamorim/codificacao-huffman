/**
 * @file huffman.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para as funções de compactação e descompactação de Huffman.
 */

#pragma once

#include "bitmap.h"
#include "heap.h"
#include "readbuffer.h"
#include "tree.h"
#include <stdio.h>

/**
 * @brief Constrói a Árvore de Huffman a partir de uma fila de prioridade (min-heap).
 * @details Esta função implementa o algoritmo de Huffman. Ela remove iterativamente
 * os dois nós de menor frequência da heap, cria um novo nó pai com a soma das
 * frequências e o insere de volta na heap. O processo continua até que reste
 * apenas um nó: a raiz da árvore de Huffman.
 *
 * @param heap A min-heap contendo os nós folha (caracteres e suas frequências).
 * @warning A heap fornecida é consumida e **liberada** ao final da execução desta função.
 * O chamador não deve mais usar o ponteiro da heap após esta chamada.
 *
 * @return Um ponteiro para a raiz da Árvore de Huffman completa.
 */
Tree *convertToHuffmanTree(Heap *heap);

/**
 * @brief Converte a Árvore de Huffman em uma tabela de consulta de códigos.
 * @details Percorre a árvore para gerar os códigos binários para cada caractere.
 * O resultado é uma tabela onde o índice é o valor do byte (0-255) e o conteúdo
 * é o seu código de Huffman correspondente, representado como um `unsigned int`.
 *
 * @param tree A raiz da Árvore de Huffman.
 * @return Um ponteiro para a tabela (array) de códigos alocada dinamicamente.
 * O chamador é responsável por liberar esta memória com `free()`.
 */
unsigned int *convertHuffmanTreeToTable(Tree *tree);

/**
 * @brief Serializa a estrutura da Árvore de Huffman em um fluxo de bits (bitmap).
 *
 * @param tree A raiz da Árvore de Huffman a ser serializada.
 * @param bitmap O bitmap onde a representação da árvore será escrita.
 */
void serializeHuffmanTree(Tree *tree, Bitmap *bitmap);

/**
 * @brief Calcula o tamanho, em bits, da representação serializada da árvore.
 * @details Retorna o número de bits necessários para armazenar a árvore serializada.
 *
 * @param tree A raiz da Árvore de Huffman.
 * @return O tamanho em bits da árvore serializada.
 */
int getSerializedHuffmanTreeSize(Tree *tree);

/**
 * @brief Processa um único bit do arquivo de entrada para descompactação.
 *
 * @param buffer O buffer de leitura de onde os bits compactados são lidos.
 * @param bitmap O bitmap de saída onde os bytes descompactados são escritos.
 * @param huffmanTree A raiz da árvore de Huffman (usada para resetar a navegação).
 * @param tree Nó atual da leitura.
 * @return O próximo nó
 */
Tree *consumeBit(ReadBuffer *buffer, Bitmap *bitmap, Tree *huffmanTree, Tree *tree);

/**
 * @brief Reconstrói a Árvore de Huffman a partir de sua representação serializada em um arquivo.
 *
 * @param buffer O buffer de leitura que contém a árvore serializada.
 * @return Um ponteiro para a raiz da Árvore de Huffman reconstruída.
 */
Tree *createHuffmanTreeFromFile(ReadBuffer *buffer);