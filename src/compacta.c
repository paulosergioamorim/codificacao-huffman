#define NOB_IMPLEMENTATION
#include "../nob.h"
#include "tree.h"
#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void print_help();

typedef struct nodes {
    int count;
    int capacity;
    Node **items;
} Nodes;

typedef struct huffman_code {
    uint32_t code;
    int len;
} Huffman_Code;

typedef struct bitmap {
    int count;
    int capacity;
    uint8_t count_bits;
    uint8_t *items;
} Bitmap;

int node_compare(const void *ptr1, const void *ptr2);

void huffman_tree_parse_to_table(Node *huffman_tree, Huffman_Code *table, uint64_t code);

void huffman_parse_to_bitmap(Node *huffman_tree, Bitmap *bitmap);

void bitmap_append_bit(Bitmap *bitmap, uint8_t bit);

void bitmap_append_byte(Bitmap *bitmap, uint8_t byte);

void bitmap_append_huffman_code(Bitmap *bitmap, Huffman_Code code);

int main(int argc, char **argv) {
    if (argc == 1) {
        print_help();
        return 1;
    }
    const char *path = argv[1];
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

    nob_log(INFO, "Loading file");
    uint8_t *buf = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (buf == MAP_FAILED) {
        printf("Failed to allocate buffer\n");
        close(fd);
        return 1;
    }

    if (madvise(buf, st.st_size, MADV_HUGEPAGE) == -1) {
        printf("Failed to use transparent huge pages\n");
    }

    nob_log(INFO, "Counting bytes frequency");
    size_t freq[UINT8_MAX + 1];
    memset(freq, 0, sizeof(freq));

    for (off_t i = 0; i < st.st_size; i++) {
        freq[buf[i]]++;
    }

    Nodes nodes = {0};

    for (int i = 0; i <= UINT8_MAX; i++) {
        if (freq[i] == 0) {
            continue;
        }
        Node *node = malloc(sizeof(*node));
        assert(node);
        node->byte = i;
        node->freq = freq[i];
        node->left = node->right = NULL;
        da_append(&nodes, node);
    }

    nob_log(INFO, "Building huffman tree");
    while (nodes.count != 1) {
        qsort(nodes.items, nodes.count, sizeof(*nodes.items), node_compare);
        Node *node = malloc(sizeof(*node));
        assert(node);
        node->byte = 0;
        node->freq = nodes.items[0]->freq + nodes.items[1]->freq;
        node->left = nodes.items[0];
        node->right = nodes.items[1];
        memmove(nodes.items, nodes.items + 2, (nodes.count - 2) * sizeof(*nodes.items));
        nodes.count -= 2;
        da_append(&nodes, node);
    }

    Node *huffman_tree = nodes.items[0];

    nob_log(INFO, "Serialize huffman tree in bitmap");
    Bitmap bitmap = {0};
    huffman_parse_to_bitmap(huffman_tree, &bitmap);

    nob_log(INFO, "Parse huffman tree to encoding table");
    Huffman_Code table[UINT8_MAX + 1];
    huffman_tree_parse_to_table(huffman_tree, table, 1);

    nob_log(INFO, "Encoding file");
    for (off_t i = 0; i < st.st_size; i++) {
        Huffman_Code huffman_code = table[buf[i]];
        // bitmap_append_huffman_code(&bitmap, huffman_code);
        for (int j = huffman_code.len - 1; j >= 0; j--) {
            if (huffman_code.code & (1 << j)) {
                bitmap_append_bit(&bitmap, 1);
            } else {
                bitmap_append_bit(&bitmap, 0);
            }
        }
    }

    if (munmap(buf, st.st_size) == -1) {
        printf("Failed to munmap buffer\n");
    }

    if (close(fd) == -1) {
        printf("Failed to close compressed file\n");
        perror(NULL);
    }

    nob_log(INFO, "Writting encoded file");
    String_Builder sb = {0};
    sb_append_cstr(&sb, path);
    sb_append_cstr(&sb, ".comp");
    sb_append_null(&sb);

    int new_fd = open(sb.items, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if (new_fd == -1) {
        printf("Failed to create new file\n");
        perror(NULL);
    }

    struct iovec vec[2];
    vec[0].iov_base = &bitmap.count_bits;
    vec[0].iov_len = sizeof(bitmap.count_bits);
    vec[1].iov_base = bitmap.items;
    vec[1].iov_len = bitmap.count;
    ssize_t bytes_written = writev(new_fd, vec, 2);

    if (bytes_written == -1) {
        printf("Failed to write in file\n");
        perror(NULL);
    }

    node_destroy(huffman_tree);
    da_free(nodes);
    da_free(bitmap);
    sb_free(sb);

    if (close(new_fd) == -1) {
        printf("Failed to close compressed file\n");
        perror(NULL);
    }

    return 0;
}

void print_help() {
    printf("./compacta <file>\n");
}

int node_compare(const void *ptr1, const void *ptr2) {
    Node *node1 = *(Node **)ptr1;
    Node *node2 = *(Node **)ptr2;
    return node1->freq - node2->freq;
}

void huffman_tree_parse_to_table(Node *node, Huffman_Code *table, uint64_t code) {
    if (!node) {
        return;
    }
    huffman_tree_parse_to_table(node->left, table, (code << 1) | 0);
    if (node_is_leaf(node)) {
        Huffman_Code huffman_code = {
            .code = code,
            .len = log2(code),
        };
        table[node->byte] = huffman_code;
    }
    huffman_tree_parse_to_table(node->right, table, (code << 1) | 1);
}

void huffman_parse_to_bitmap(Node *node, Bitmap *bitmap) {
    if (!node) {
        return;
    }

    if (bitmap->items == NULL) {
        da_append(bitmap, 0);
    }

    if (node_is_leaf(node)) {
        bitmap_append_bit(bitmap, 1);
        for (int i = 7; i >= 0; i--) {
            if (node->byte & (1 << i)) {
                bitmap_append_bit(bitmap, 1);
            } else {
                bitmap_append_bit(bitmap, 0);
            }
        }
        return;
    }

    bitmap_append_bit(bitmap, 0);
    huffman_parse_to_bitmap(node->left, bitmap);
    huffman_parse_to_bitmap(node->right, bitmap);
}

void bitmap_append_bit(Bitmap *bitmap, uint8_t bit) {
    if (bitmap->count_bits == 8) {
        bitmap->count_bits = 0;
        da_append(bitmap, 0);
    }
    bitmap->items[bitmap->count - 1] |= (1 & bit) << (7 - bitmap->count_bits);
    bitmap->count_bits++;
}

void bitmap_append_byte(Bitmap *bitmap, uint8_t byte) {
    if (bitmap->count_bits == 0) {
        da_append(bitmap, byte);
        return;
    }

    bitmap->items[bitmap->count - 1] |= (byte >> bitmap->count_bits);
    da_append(bitmap, 0);
    bitmap->items[bitmap->count - 1] |= (byte << (7 - bitmap->count_bits));
}

void bitmap_append_huffman_code(Bitmap *bitmap, Huffman_Code hc) {
    NOB_UNUSED(bitmap);
    NOB_UNUSED(hc);
    NOB_TODO(__FUNCTION__);
}
