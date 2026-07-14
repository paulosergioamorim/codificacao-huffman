#include <stdint.h>
#define NOB_IMPLEMENTATION
#include "../nob.h"
#include "tree.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>

void print_help();

Node *node_build_from(uint8_t *buf, int *index_bytes, int *index_bits);

int main(int argc, char **argv) {
    if (argc == 1) {
        print_help();
        return 1;
    }
    const char *path = argv[1];
    String_View ext_sv = sv_from_cstr(".comp");
    String_View path_sv = sv_from_cstr(path);

    if (!sv_ends_with(path_sv, ext_sv)) {
        printf("Invalid format\n");
        return 1;
    }

    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        printf("Failed to open file %s\n", path);
        return 1;
    }

    struct stat st = {0};
    if (fstat(fd, &st) == -1) {
        printf("Failed to stat file %s\n", path);
        close(fd);
        return 1;
    }

    uint8_t *buf = (uint8_t *)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (buf == MAP_FAILED) {
        printf("Failed to allocate buffer\n");
        close(fd);
        return 1;
    }

    if (madvise(buf, st.st_size, MADV_HUGEPAGE) == -1) {
        printf("Failed to use transparent huge pages\n");
    }

    int index_bytes = 1;
    int index_bits = 0;
    Node *huffman_tree = node_build_from(buf, &index_bytes, &index_bits);

    node_display(huffman_tree);

    return 0;
}

void print_help() {
    printf("./descompacta <file>");
}

Node *node_build_from(uint8_t *buf, int *index_bytes, int *index_bits) {
    uint8_t bit = 1 & (buf[*index_bytes] >> (7 - *index_bits));
    (*index_bits)++;
    if (*index_bits == 7) {
        *index_bits = 0;
        (*index_bytes)++;
    }
    Node *node = malloc(sizeof(*node));
    memset(node, 0, sizeof(*node));
    if (bit == 0) {
        node->left = node_build_from(buf, index_bytes, index_bits);
        node->right = node_build_from(buf, index_bytes, index_bits);
    } else {
        uint8_t byte = 0;
        for (int i = 7; i >= 0; i--) {
            bit = 1 & (buf[*index_bytes] >> (7 - *index_bits));
            byte |= (bit << i);
            (*index_bits)++;
            if (*index_bits == 7) {
                *index_bits = 0;
                (*index_bytes)++;
            }
        }
        node->byte = byte;
    }
    return node;
}
