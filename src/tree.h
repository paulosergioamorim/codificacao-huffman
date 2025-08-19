/**
 * @file tree.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para TAD de Árvore do projeto Compactador e Descompactador.
 */

#pragma once

/** @brief Tipo opaco da árvore. */
typedef struct tree Tree;

/**
 * @brief Cria um novo nó de árvore.
 * @param value Byte armazenado no nó (0x00 se não for folha).
 * @param frequency Frequência do símbolo ou soma das frequências dos filhos.
 * @return Ponteiro para o nó criado.
 */
Tree *createTree(unsigned char value, int frequency);

/**
 * @brief Define o filho esquerdo do nó.
 * @param tree Nó pai.
 * @param left Filho esquerdo (pode ser NULL).
 */
void setLeftTree(Tree *tree, Tree *left);

/**
 * @brief Retorna o filho esquerdo do nó.
 * @param tree Nó atual.
 * @return Ponteiro para o filho esquerdo ou NULL.
 */
Tree *getLeftTree(Tree *tree);

/**
 * @brief Define o filho direito do nó.
 * @param tree Nó pai.
 * @param right Filho direito (pode ser NULL).
 */
void setRightTree(Tree *tree, Tree *right);

/**
 * @brief Retorna o filho direito do nó.
 * @param tree Nó atual.
 * @return Ponteiro para o filho direito ou NULL.
 */
Tree *getRightTree(Tree *tree);

/**
 * @brief Verifica se o nó é uma folha.
 * @param tree Nó a ser verificado (pode ser NULL).
 * @return 1 se for folha, 0 caso contrário.
 */
int isLeafTree(Tree *tree);

/**
 * @brief Conta a quantidade de folhas na árvore.
 * @param tree Raiz da árvore (pode ser NULL).
 * @return Número total de folhas.
 */
int getLeafNodesCountTree(Tree *tree);

/**
 * @brief Retorna o valor armazenado no nó.
 * @param tree Nó.
 * @return Byte associado ao nó.
 */
unsigned char getValueTree(Tree *tree);

/**
 * @brief Retorna a frequência associada ao nó.
 * @param tree Nó.
 * @return Frequência do nó.
 */
int getFrequencyTree(Tree *tree);

/**
 * @brief Libera recursivamente todos os nós da árvore.
 * @param tree Raiz da árvore (pode ser NULL).
 */
void freeTree(Tree *tree);

/**
 * @brief Compara as frequências de dois nós.
 * @param tree1 Primeiro nó.
 * @param tree2 Segundo nó.
 * @return 1 se freq(tree1) >= freq(tree2), 0 caso contrário.
 */
int compareFrequencyTrees(Tree *tree1, Tree *tree2);

/**
 * @brief Imprime a árvore em pré-ordem com delimitadores.
 * @param tree Raiz da árvore (pode ser NULL).
 * @details Formato: imprime "<"; se folha, imprime o caractere/byte;
 * em seguida os filhos esquerdo e direito; por fim imprime ">".
 */
void printTree(Tree *tree);
