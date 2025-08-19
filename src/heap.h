/**
 * @file heap.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para o TAD de Min Heap de Árvores.
 */

#pragma once

#include "tree.h"

/**
 * @typedef Heap
 * @brief Tipo opaco que representa a estrutura da Min Heap de Árvores.
 */
typedef struct heap Heap;

/**
 * @brief Cria uma Min Heap vazia com capacidade máxima.
 * @param maxCapacity Capacidade máxima de elementos.
 * @return Ponteiro para a heap criada, ou NULL em caso de falha.
 */
Heap *createHeap(int maxCapacity);

/**
 * @brief Insere um nó de árvore na heap.
 * @param heap Heap de destino.
 * @param tree Nó da árvore a ser inserido.
 */
void pushHeap(Heap *heap, Tree *tree);

/**
 * @brief Remove e retorna o nó de menor frequência da heap.
 * @param heap Heap de onde será removido o nó.
 * @return Ponteiro para o nó removido, ou NULL se a heap estiver vazia.
 */
Tree *popHeap(Heap *heap);

/**
 * @brief Libera a memória alocada pela heap.
 * @param heap Heap a ser liberada.
 * @details Os nós da árvore não são liberados.
 */
void freeHeap(Heap *heap);

/**
 * @brief Imprime o conteúdo da heap.
 * @param heap Heap a ser impressa.
 */
void printHeap(Heap *heap);

/**
 * @brief Constrói a Árvore de Huffman a partir dos nós da heap.
 * @param heap Heap contendo os nós folha.
 * @return Ponteiro para a raiz da árvore de Huffman.
 * @details A heap é consumida e liberada ao final.
 */
Tree *convertHeapToHuffmanTree(Heap *heap);
