/*
 * UFES - Universidade Federal do Espírito Santo
 * Autor: Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 * Projeto: Compactador e Descompactador
 */

#pragma once

#include "bitmap.h"
#include "heap.h"
#include "readbuffer.h"
#include "tree.h"
#include <stdio.h>

Tree *convertToHuffmanTree(Heap *heap);

unsigned int *convertHuffmanTreeToTable(Tree *tree);

void serializeHuffmanTree(Tree *tree, Bitmap *bitmap);

int getSerializedHuffmanTreeSize(Tree *tree);

void consumeBit(ReadBuffer *buffer, Bitmap *bitmap, Tree *huffmanTree, Tree **tree);

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer);
