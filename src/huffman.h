#pragma once

#include "tree.h"
#include "heap.h"
#include "arraybyte.h"
#include <stdio.h>

#ifdef ENCODER_PROGRAM
Tree *convertToHuffmanTree(Heap *heap);

ArrayByte **convertHuffmanTreeToTable(Tree *tree);

void freeEncodingTable(ArrayByte **table);

void saveHuffmanTreeToFile(Tree *tree, FILE *fp);
#endif

#ifdef DECODER_PROGRAM
Tree *createHuffmanTreeFromFile(FILE *fp);
#endif
