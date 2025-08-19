/**
 * @file huffman.h
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * @brief Cabeçalho para as funções de compactação e descompactação de Huffman.
 */

#pragma once

#include "bitmap.h"
#include "readbuffer.h"
#include "tree.h"
#include <stdio.h>

/**
 * @brief Gera a tabela de códigos a partir da Árvore de Huffman.
 * @param tree Raiz da árvore de Huffman.
 * @return Ponteiro para um array com os códigos gerados (alocado dinamicamente).
 */
unsigned int *convertHuffmanTreeToTable(Tree *tree);

/**
 * @brief Serializa a Árvore de Huffman para um fluxo de bits.
 * @param tree Raiz da árvore a ser serializada.
 * @param bitmap Bitmap onde os bits da árvore serão armazenados.
 */
void serializeHuffmanTree(Tree *tree, Bitmap *bitmap);

/**
 * @brief Calcula o número de bits necessários para representar a árvore serializada.
 * @param tree Raiz da árvore de Huffman.
 * @return Quantidade de bits da serialização.
 */
int getSerializedHuffmanTreeSize(Tree *tree);

/**
 * @brief Processa um bit da entrada compactada e avança na árvore de Huffman.
 * @param buffer Buffer de leitura contendo os bits do arquivo compactado.
 * @param bitmap Bitmap onde os bytes descompactados serão escritos.
 * @param huffmanTree Raiz da árvore de Huffman (ponto de reinício da navegação).
 * @param tree Nó atual da árvore na leitura.
 * @return Próximo nó após processar o bit.
 */
Tree *consumeBit(ReadBuffer *buffer, Bitmap *bitmap, Tree *huffmanTree, Tree *tree);

/**
 * @brief Reconstrói a árvore de Huffman a partir da sua forma serializada.
 * @param buffer Buffer contendo os dados da árvore serializada.
 * @return Ponteiro para a raiz da árvore reconstruída.
 */
Tree *createHuffmanTreeFromFile(ReadBuffer *buffer);
