#pragma once

#include "tree.h"

typedef struct queue Queue;

Queue *CreateQueue();

void Enqueue(Queue *queue, Tree *tree);

Tree *Dequeue(Queue *queue);

void FreeQueue(Queue *queue);

void PrintQueue(Queue *queue);

int ExistsQueue(Queue *queue, int key);

Tree *ToHuffmanTree(Queue *queue);
