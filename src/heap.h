#pragma once
#include "tree.h"

typedef struct heap Heap;

Heap *createHeap(int maxCapacity);

void pushHeap(Heap *heap, Tree *tree);

Tree *popHeap(Heap *heap);

void freeHeap(Heap *heap);

void printHeap(Heap *heap);

int getSizeHeap(Heap *heap);