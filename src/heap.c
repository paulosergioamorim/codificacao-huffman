/**
 * @file heap.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 */

#include "heap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
    int i = heap->size++; // i é o novo último índice e aumenta o tamanho

    while (i > 0)
    {
        int parentIndex = (i - 1) / 2;
        Tree *parent = heap->vec[parentIndex];

        if (compareFrequencyTrees(tree, parent))
            break; // caso: i deve ser filho de parent

        // caso: parent na verdade é filho de i
        heap->vec[i] = parent;
        i = parentIndex; // parent é o novo i
    } // balanceamento da heap

    heap->vec[i] = tree; // posição final do novo elemento
}

Tree *popHeap(Heap *heap)
{
    if (heap->size == 0)
        return NULL;

    Tree *min = heap->vec[0];
    heap->vec[0] = heap->vec[--heap->size]; // diminui o tamanho e move o último elemento da heap para o início
    int i = 0;

    while (1)
    {
        int leftIndex = 2 * i + 1;  // índice filho essquerdo de i
        int rightIndex = 2 * i + 2; // índice filho direito de i
        int smallestIndex = i;      // caso: suponha que o menor seja i = 0

        if (leftIndex < heap->size && compareFrequencyTrees(heap->vec[smallestIndex], heap->vec[leftIndex]))
            smallestIndex = leftIndex; // caso: o filho esquerdo de i na verdade é menor que i

        if (rightIndex < heap->size && compareFrequencyTrees(heap->vec[smallestIndex], heap->vec[rightIndex]))
            smallestIndex = rightIndex; // caso: o filho direito de i na verdade é menor que i

        if (smallestIndex == i)
            break; // caso: a heap está balanceada

        // Agora, já sabemos os índices que devem realizar a troca
        Tree *temp = heap->vec[i];
        heap->vec[i] = heap->vec[smallestIndex];
        heap->vec[smallestIndex] = temp;
        i = smallestIndex; // o menor é o novo i
    } // balanceamento da heap

    return min;
}

void freeHeap(Heap *heap)
{
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

Tree *convertHeapToHuffmanTree(Heap *heap)
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