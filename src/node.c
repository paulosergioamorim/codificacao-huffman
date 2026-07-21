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

void node_display(Node *node) {
    if (!node) {
        return;
    }
    node_display(node->left);
    if (node_is_leaf(node)) {
        if (isascii(node->byte)) {
            printf("%c %ld\n", node->byte, node->freq);
        } else {
            printf("0x%x %ld\n", node->byte, node->freq);
        }
    }
    node_display(node->right);
}
