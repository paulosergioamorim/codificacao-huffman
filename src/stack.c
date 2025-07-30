#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

typedef struct node
{
    Tree *value;
    struct node *next;
} Node;

struct stack
{
    Node *top;
};

Stack *stackInit()
{
    Stack *stack = malloc(sizeof(stack));
    stack->top = NULL;

    return stack;
}

Tree *stackPop(Stack *stack)
{
    if (!stack->top)
        return NULL;

    Tree *cur = stack->top->value;
    stack->top = stack->top->next;

    return cur;
}

void stackPush(Stack *stack, Tree *tree)
{
    Node *node = malloc(sizeof(Node));
    node->value = tree;
    node->next = NULL;

    if (!stack->top)
    {
        stack->top = node;
        return;
    }

    node->next = stack->top;
    stack->top = node;
}

void stackFree(Stack *stack)
{
    Node *cur = stack->top;

    while (!cur)
    {
        Node *prev = cur;
        cur = cur->next;
        free(prev);
    }

    free(stack);
}