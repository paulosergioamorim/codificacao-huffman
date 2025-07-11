#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tree.h"
#include "queue.h"

typedef struct node
{
    Tree *value;
    struct node *prev;
    struct node *next;
} Node;

struct queue
{
    Node *first;
    Node *last;
};

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
    Node *node = malloc(sizeof(Node));
    assert(node);
    node->value = tree;
    node->next = node->prev = NULL;

    if (!queue->first)
    {
        queue->first = queue->last = node;
        return;
    }

    int weigth = GetWeigthTree(tree);

    Node *cur = queue->first;

    while (cur && weigth >= GetWeigthTree(cur->value))
    {
        cur = cur->next;
    }

    if (!cur)
    {
        node->prev = queue->last;
        queue->last->next = node;
        queue->last = node;
        return;
    }

    if (!cur->prev)
    {
        cur->prev = node;
        node->next = cur;
        queue->first = node;
        return;
    }

    node->prev = cur->prev;
    node->next = cur;
    cur->prev->next = node;
    cur->prev = node;
}

Tree *Dequeue(Queue *queue)
{
    assert(queue);

    if (!queue->first)
        return NULL;

    Node *node = queue->first;

    queue->first = node->next;

    if (queue->first)
        queue->first->prev = NULL;

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
        free(next);
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

        if (IsLeafTree(tree))
            printf("%c ", GetValueTree(tree));
        else
            printf("%d ", GetWeigthTree(tree));

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
