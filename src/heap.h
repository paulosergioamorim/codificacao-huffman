#pragma once
#include "tree.h"

#define HEAP_INITIAL_CAPACITY 64;

typedef struct heap Heap;

Heap *createHeap();

void pushHeap(Heap *heap, Tree *tree);

Tree *popHeap(Heap *heap);

void freeHeap(Heap *heap);

void printHeap(Heap *heap);

Tree *convertToHuffmanTree(Heap *heap);