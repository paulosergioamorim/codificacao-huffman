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

Heap *createHeap()
{
    Heap *heap = malloc(sizeof(Heap));
    assert(heap);
    heap->capacity = HEAP_INITIAL_CAPACITY;
    heap->size = 0;
    heap->vec = malloc(heap->capacity * sizeof(Tree *));
    assert(heap->vec);

    return heap;
}

void pushHeap(Heap *heap, Tree *tree)
{
    assert(heap);
    if (heap->size == heap->capacity)
    {
        heap->capacity *= 2;
        heap->vec = realloc(heap->vec, heap->capacity * sizeof(Tree *));
        assert(heap->vec);
    }

    int i = heap->size++;

    while (i > 0)
    {
        int parentIndex = (i - 1) / 2;
        Tree *parent = heap->vec[parentIndex];

        if (compareTrees(tree, parent))
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
            compareTrees(heap->vec[smallest], heap->vec[left]))
            smallest = left;

        if (right < heap->size &&
            compareTrees(heap->vec[smallest], heap->vec[right]))
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
    {
        freeTree(heap->vec[i]);
    }

    free(heap->vec);
    free(heap);
}

void printHeap(Heap *heap)
{
    printf("[");

    for (int i = 0; i < heap->size; i++)
    {
        Tree *tree = heap->vec[i];
        printf("(%c, %d)", getValueTree(tree), getFrequencyTree(tree));

        if (i < heap->size - 1)
            printf(" ");
    }

    printf("]\n");
}

Tree *convertToHuffmanTree(Heap *heap)
{
    while (heap->size > 1)
    {
        Tree *tree1 = popHeap(heap);
        Tree *tree2 = popHeap(heap);
        Tree *tree3 = createTree(0, getFrequencyTree(tree1) + getFrequencyTree(tree2));
        setLeftTree(tree3, tree1);
        setRightTree(tree3, tree2);
        pushHeap(heap, tree3);
    }

    Tree *huffmanTree = popHeap(heap);
    freeHeap(heap);

    return huffmanTree;
}