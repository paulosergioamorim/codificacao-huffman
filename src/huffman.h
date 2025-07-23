#pragma once

#include "tree.h"
#include "heap.h"
#include "bitarray.h"
#include <stdio.h>

Tree *convertToHuffmanTree(Heap *heap);

BitArray **convertHuffmanTreeToTable(Tree *tree);

void freeEncodingTable(BitArray **table);

void serializeHuffmanTree(Tree *tree, BitArray *array);

float getExpectedHeightHuffmanTree(int totalBytes, Tree *tree);
