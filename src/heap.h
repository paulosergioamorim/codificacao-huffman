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
 * @brief Cria e aloca uma nova Min Heap vazia.
 * @param maxCapacity A capacidade máxima de elementos que a heap poderá armazenar.
 * @return Um ponteiro para a estrutura `Heap` recém-criada, ou NULL em caso de falha.
 */
Heap *createHeap(int maxCapacity);

/**
 * @brief Insere um nó de árvore na heap, mantendo a propriedade de min-heap.
 * @param heap A heap na qual o elemento será inserido.
 * @param tree O ponteiro para o nó da árvore a ser adicionado.
 */
void pushHeap(Heap *heap, Tree *tree);

/**
 * @brief Remove e retorna o elemento de menor frequência da heap (a raiz).
 * @param heap A heap da qual o elemento será removido.
 * @return Um ponteiro para o nó da árvore com a menor frequência. Retorna NULL se a heap estiver vazia.
 */
Tree *popHeap(Heap *heap);

/**
 * @brief Libera a memória alocada para a estrutura da heap.
 * @warning Esta função libera a estrutura da heap e seu array interno, mas **não**
 * libera a memória dos ponteiros `Tree*` que ela contém. O gerenciamento
 * da memória dos nós da árvore é de responsabilidade externa, pois os nós
 * extraídos são usados para construir a Árvore de Huffman final.
 * @param heap A heap a ser liberada.
 */
void freeHeap(Heap *heap);

/**
 * @brief Imprime o conteúdo da heap.
 * @param heap A heap a ser impressa.
 */
void printHeap(Heap *heap);

/**
 * @brief Retorna o número atual de elementos na heap.
 * @param heap A heap a ser consultada.
 * @return O número de elementos presentes na heap.
 */
int getSizeHeap(Heap *heap);