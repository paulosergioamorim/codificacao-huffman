#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "queue.h"

typedef struct node
{
    Tree *value;
    struct node *next;
} Node;

struct queue
{
    Node *first;
    Node *last;
};

static Node *CreateNode(Tree *tree);

Queue *CreateQueue()
{
    Queue *queue = malloc(sizeof(Queue));
    assert(queue);
    queue->first = queue->last = NULL;

    return queue;
}

void Enqueue(Queue *queue, Tree *tree)
{
    assert(queue);
    Node *node = CreateNode(tree);
    Node *prev = NULL;
    Node *cur = queue->first;

    if (!queue->first)
    {
        queue->first = queue->last = node;
        return;
    }

    int frequency = GetFrequencyTree(tree);

    while (cur && frequency >= GetFrequencyTree(cur->value))
    {
        prev = cur;
        cur = cur->next;
    }

    if (!prev)
    {
        node->next = queue->first;
        queue->first = node;

        if (!queue->last)
            queue->last = node;

        return;
    }

    if (cur)
    {
        prev->next = node;
        node->next = cur;
        return;
    }

    prev->next = node;
    queue->last = node;
}

Tree *Dequeue(Queue *queue)
{
    assert(queue);

    if (!queue->first)
        return NULL;

    Node *node = queue->first;
    queue->first = node->next;
    Tree *tree = node->value;
    free(node);

    return tree;
}

void FreeQueue(Queue *queue)
{
    Node *cur = queue->first;

    while (cur)
    {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }

    free(queue);
}

void PrintQueue(Queue *queue)
{
    assert(queue);
    Node *cur = queue->first;

    while (cur)
    {
        Tree *tree = cur->value;

        printf("%c -> ", GetValueTree(tree));

        cur = cur->next;
    }

    printf("\n");
}

int ExistsQueue(Queue *queue, int key)
{
    assert(queue);
    Node *cur = queue->first;

    while (cur)
    {
        if (GetValueTree(cur->value) == key)
            return 1;

        cur = cur->next;
    }

    return 0;
}

Tree *ToHuffmanTree(Queue *queue)
{
    assert(queue);
    Tree *huffmanTree = NULL;
    Tree *tree1 = NULL;
    Tree *tree2 = NULL;

    while ((tree1 = Dequeue(queue)))
    {
        if (!(tree2 = Dequeue(queue)))
        {
            FreeQueue(queue);
            return tree1;
        }

        int frequency = GetFrequencyTree(tree1) + GetFrequencyTree(tree2);
        huffmanTree = CreateTree(0, frequency, tree1, tree2);
        Enqueue(queue, huffmanTree);
    }

    FreeQueue(queue);

    return huffmanTree;
}

static Node *CreateNode(Tree *tree)
{
    assert(tree);
    Node *node = malloc(sizeof(Node));
    assert(node);
    node->value = tree;
    node->next = NULL;

    return node;
}