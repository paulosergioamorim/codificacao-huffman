#pragma once

#include "tree.h"
#include "heap.h"
#include "arraybyte.h"
#include <stdio.h>

Tree *convertToHuffmanTree(Heap *heap);

ArrayByte **convertHuffmanTreeToTable(Tree *tree);

void freeEncodingTable(ArrayByte **table);

void saveHuffmanTreeToFile(Tree *tree, FILE *fp);

Tree *createHuffmanTreeFromFile(FILE *fp);
