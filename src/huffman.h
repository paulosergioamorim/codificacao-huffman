#pragma once

#include "tree.h"
#include "heap.h"
#include "bitarray.h"
#include "readbuffer.h"
#include <stdio.h>

Tree *convertToHuffmanTree(Heap *heap);

unsigned int *convertHuffmanTreeToTable(Tree *tree);

void freeEncodingTable(unsigned int *table);

void serializeHuffmanTree(Tree *tree, BitArray *array);

int getSerializedHuffmanTreeSize(Tree *tree);

void consumeBit(ReadBuffer *buffer, BitArray *array, Tree *huffmanTree, Tree **tree);

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer);
