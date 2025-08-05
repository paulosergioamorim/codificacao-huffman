#pragma once

#include "bitarray.h"
#include "heap.h"
#include "readbuffer.h"
#include "tree.h"
#include <stdio.h>

Tree *convertToHuffmanTree(Heap *heap);

unsigned int *convertHuffmanTreeToTable(Tree *tree);

void serializeHuffmanTree(Tree *tree, BitArray *array);

int getSerializedHuffmanTreeSize(Tree *tree);

void consumeBit(ReadBuffer *buffer, BitArray *array, Tree *huffmanTree, Tree **tree);

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer);
