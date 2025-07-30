#pragma once

#include "tree.h"

typedef struct stack Stack;

Stack *stackInit();

Tree *stackPop(Stack *stack);

void stackPush(Stack *stack, Tree *tree);

void stackFree(Stack *stack);