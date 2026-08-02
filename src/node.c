#include "node.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void node_destroy(Node *node) {
    if (!node) {
        return;
    }
    node_destroy(node->left);
    node_destroy(node->right);
    free(node);
}

void node_display(const Node *node) {
    if (!node) {
        return;
    }
    if (node_is_leaf(node)) {
        if (isascii(node->byte)) {
            printf("%c", node->byte);
            return;
        }
        printf("0x%x", node->byte);
        return;
    }
    printf("<");
    node_display(node->left);
    printf(",");
    node_display(node->right);
    printf(">");
}
