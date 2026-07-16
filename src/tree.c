#include "tree.h"
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
        printf("%c %ld\n", node->byte, node->freq);
    }
    node_display(node->right);
}
