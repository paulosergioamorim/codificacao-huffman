#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"

struct heap
{
    Tree **vec;
    int size;
    int capacity;
};

Heap *createHeap(int maxCapacity)
{
    Heap *heap = malloc(sizeof(Heap));
    assert(heap);
    heap->capacity = maxCapacity;
    heap->size = 0;
    heap->vec = malloc(heap->capacity * sizeof(Tree *));
    assert(heap->vec);

    return heap;
}

void pushHeap(Heap *heap, Tree *tree)
{
    assert(heap);
    assert(heap->size < heap->capacity);
    int i = heap->size++;

    while (i > 0)
    {
        int parentIndex = (i - 1) / 2;
        Tree *parent = heap->vec[parentIndex];

        if (compareFrequencyTrees(tree, parent))
            break;

        heap->vec[i] = parent;
        i = parentIndex;
    }

    heap->vec[i] = tree;
}

Tree *popHeap(Heap *heap)
{
    assert(heap);
    if (heap->size == 0)
        return NULL;

    Tree *min = heap->vec[0];

    heap->vec[0] = heap->vec[--heap->size];

    int i = 0;

    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < heap->size &&
            compareFrequencyTrees(heap->vec[smallest], heap->vec[left]))
            smallest = left;

        if (right < heap->size &&
            compareFrequencyTrees(heap->vec[smallest], heap->vec[right]))
            smallest = right;

        if (smallest == i)
            break;

        Tree *temp = heap->vec[i];
        heap->vec[i] = heap->vec[smallest];
        heap->vec[smallest] = temp;
        i = smallest;
    }

    return min;
}

void freeHeap(Heap *heap)
{
    assert(heap);

    for (int i = 0; i < heap->size; i++)
        freeTree(heap->vec[i]);

    free(heap->vec);
    free(heap);
}

void printHeap(Heap *heap)
{
    printf("[");

    for (int i = 0; i < heap->size; i++)
    {
        Tree *tree = heap->vec[i];
        printf("%c", getValueTree(tree));

        if (i < heap->size - 1)
            printf(", ");
    }

    printf("]\n");
}

int getSizeHeap(Heap *heap)
{
    return heap->size;
}