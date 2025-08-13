/**
 * @file tree.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para TAD de Árvore do projeto Compactador e Descompactador.
 */

#pragma once

/** Tipo opaco da árvore. */
typedef struct tree Tree;

/**
 * @brief Cria um nó de árvore.
 * @param value Byte do nó (0x00 se não for folha).
 * @param frequency Frequência do símbolo (se for folha) ou soma dos filhos (caso contrário).
 * @return Ponteiro para o novo nó.
 */
Tree *createTree(unsigned char value, int frequency);

/**
 * @brief Define o filho esquerdo.
 * @param tree Nó pai.
 * @param left Filho esquerdo (pode ser NULL).
 */
void setLeftTree(Tree *tree, Tree *left);

/**
 * @brief Retorna o filho esquerdo.
 * @param tree Nó.
 * @return Filho esquerdo ou NULL.
 */
Tree *getLeftTree(Tree *tree);

/**
 * @brief Define o filho direito.
 * @param tree Nó pai.
 * @param right Filho direito (pode ser NULL).
 */
void setRightTree(Tree *tree, Tree *right);

/**
 * @brief Retorna o filho direito.
 * @param tree Nó.
 * @return Filho direito ou NULL.
 */
Tree *getRightTree(Tree *tree);

/**
 * @brief Informa se o nó é folha.
 * @param tree Nó a testar (pode ser NULL).
 * @return 1 se não NULL e sem filhos; 0 caso contrário.
 */
int isLeafTree(Tree *tree);

/**
 * @brief Conta o número de folhas da árvore.
 * @param tree Raiz (pode ser NULL).
 * @return Quantidade de folhas.
 * @note Implementação recursiva.
 */
int getLeafNodesCountTree(Tree *tree);

/**
 * @brief Obtém o valor (byte) do nó.
 * @param tree Nó.
 * @return Valor do nó.
 */
unsigned char getValueTree(Tree *tree);

/**
 * @brief Obtém a frequência do nó.
 * @param tree Nó.
 * @return Frequência.
 */
int getFrequencyTree(Tree *tree);

/**
 * @brief Libera recursivamente todos os nós da árvore.
 * @param tree Raiz (pode ser NULL).
 */
void freeTree(Tree *tree);

/**
 * @brief Compara frequências de dois nós.
 * @param tree1 Primeiro nó.
 * @param tree2 Segundo nó.
 * @return 1 se freq(tree1) >= freq(tree2); 0 caso contrário.
 * @note Predicado booleano; não é comparador de três vias para `qsort`.
 */
int compareFrequencyTrees(Tree *tree1, Tree *tree2);

/**
 * @brief Imprime a árvore em pré-ordem com delimitadores.
 * @param tree Raiz (pode ser NULL).
 * @details Formato: imprime "<"; se folha, imprime `%c` do valor;
 *          depois esquerda, direita; e por fim ">".
 * @note Escreve em `stdout` e não adiciona `\\n`.
 */
void printTree(Tree *tree);
