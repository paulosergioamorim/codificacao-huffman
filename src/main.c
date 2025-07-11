#include "queue.h"
#include <string.h>
#include <stdio.h>

int CountCharStr(char *str, char c);

int main(int argc, char const *argv[])
{
    Queue *queue = CreateQueue();
    char *str = "bom esse bombom";

    for (int i = 0; i < strlen(str); i++)
    {
        char c = str[i];
        int count = CountCharStr(str, c);

        if (!ExistsQueue(queue, c))
            Enqueue(queue, CreateTree(c, count));
    }

    Tree *huffmanTree = NULL;

    while (1)
    {
        Tree *tree1 = Dequeue(queue);
        Tree *tree2 = Dequeue(queue);

        if (!tree1 || !tree2)
            break;

        huffmanTree = CreateTree(0, GetWeigthTree(tree1) + GetWeigthTree(tree2));
        SetLeftTree(huffmanTree, tree1);
        SetRightTree(huffmanTree, tree2);
        Enqueue(queue, huffmanTree);
    }

    PrintTree(huffmanTree);
    printf("\n");
    FreeQueue(queue);
    FreeTree(huffmanTree);

    return 0;
}

int CountCharStr(char *str, char c)
{
    int count = 0;

    while (*str)
    {
        if (*str == c)
            count++;

        str++;
    }

    return count;
}
