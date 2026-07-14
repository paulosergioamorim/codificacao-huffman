#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct node Node;

struct node {
    uint8_t byte;
    off_t freq;
    Node *left;
    Node *right;
};

void node_destroy(Node *node);

void node_display(Node *node);

static inline bool node_is_leaf(Node *node) {
    return node->left == NULL && node->right == NULL;
}

#endif // TREE_H
