#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct node {
    uint8_t byte;
    off_t freq;
    struct node *left;
    struct node *right;
} Node;

void node_destroy(Node *node);

void node_display(Node *node);

static inline bool node_is_leaf(Node *node) {
    return node->left == NULL && node->right == NULL;
}

#endif // NODE_H
