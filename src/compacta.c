#define NOB_IMPLEMENTATION
#include "../nob.h"
#include "file_header.h"
#include "node.h"
#include <assert.h>
#include <fcntl.h>
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
    uint64_t code;
    int len;
} Huffman_Code;

typedef struct bitmap {
    int count;
    int capacity;
    uint8_t count_bits;
    uint8_t *items;
} Bitmap;

int node_compare(const void *ptr1, const void *ptr2);

void huffman_tree_parse_to_table(Node *huffman_tree, Huffman_Code *table, uint64_t code, int len);

void huffman_table_display(Huffman_Code *table);

void bitmap_append_huffman_tree(Bitmap *bitmap, Node *huffman_tree);

void bitmap_append_bit(Bitmap *bitmap, uint8_t bit);

void bitmap_append_byte(Bitmap *bitmap, uint8_t byte);

void bitmap_append_huffman_code(Bitmap *bitmap, Huffman_Code hc);

int main(int argc, char **argv) {
    if (argc == 1) {
        print_help();
        return 0;
    }

    const char *path = argv[1];
    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        nob_log(ERROR, "Failed to open file");
        return 1;
    }

    struct stat st = {0};
    if (fstat(fd, &st) == -1) {
        nob_log(ERROR, "Failed to stat file");
        return 1;
    }

    String_Builder sb = {0};
    sb_append_cstr(&sb, path);
    sb_append_cstr(&sb, ".comp");
    sb_append_null(&sb);

    File_Header header = {0};
    memcpy(header.magic_bytes, magic_bytes, sizeof(magic_bytes));
    int new_fd = open(sb.items, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (new_fd == -1) {
        nob_log(ERROR, "Failed to create new file");
        sb_free(sb);
        close(fd);
        return 1;
    }

    if (st.st_size == 0) {
        header.count_last_valid_bits = 0;
        ssize_t bytes_written = write(new_fd, &header, sizeof(header));
        if (bytes_written == -1) {
            nob_log(ERROR, "Failed to write file");
        }
        sb_free(sb);
        close(fd);
        close(new_fd);
        return 0;
    } // empty file

    uint8_t *buf = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (buf == MAP_FAILED) {
        nob_log(ERROR, "Failed mmap buffer");
        sb_free(sb);
        close(fd);
        close(new_fd);
        return 1;
    }

    if (madvise(buf, st.st_size, MADV_HUGEPAGE) == -1) {
        nob_log(WARNING, "Not using transparent huge pages");
    }

    off_t freq[UINT8_MAX + 1] = {0};

    nob_log(INFO, "Counting byte frequencies");
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

    nob_log(INFO, "Building Huffman Tree");
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

    Bitmap bitmap = {0};
    bitmap_append_huffman_tree(&bitmap, huffman_tree);

    Huffman_Code table[UINT8_MAX + 1] = {0};
    huffman_tree_parse_to_table(huffman_tree, table, 0, 0);

    if (node_is_leaf(huffman_tree)) {
        header.is_root_leaf = true;
        for (off_t i = 0; i < st.st_size; i++) {
            bitmap_append_bit(&bitmap, 0);
        }
    } // unique byte

    nob_log(INFO, "Encoding file");
    for (off_t i = 0; i < st.st_size; i++) {
        Huffman_Code huffman_code = table[buf[i]];
        bitmap_append_huffman_code(&bitmap, huffman_code);
    }

    if (munmap(buf, st.st_size) == -1) {
        nob_log(ERROR, "Failed to munmap buffer");
    }

    if (close(fd) == -1) {
        nob_log(ERROR, "Failed to close file");
    }

    header.count_last_valid_bits = bitmap.count_bits;

    nob_log(INFO, "Write new file");
    struct iovec vec[2];
    vec[0].iov_base = &header;
    vec[0].iov_len = sizeof(header);
    vec[1].iov_base = bitmap.items;
    vec[1].iov_len = bitmap.count * sizeof(*bitmap.items);
    ssize_t bytes_written = writev(new_fd, vec, 2);

    if (bytes_written == -1) {
        nob_log(ERROR, "Failed to write new file");
    }

    node_destroy(huffman_tree);
    da_free(nodes);
    da_free(bitmap);
    sb_free(sb);

    if (close(new_fd) == -1) {
        nob_log(ERROR, "Failed to close new file");
    }

    return 0;
}

void print_help() {
    printf("USAGE:\n"
           "\t./compacta <file>\n");
}

int node_compare(const void *ptr1, const void *ptr2) {
    Node *node1 = *(Node **)ptr1;
    Node *node2 = *(Node **)ptr2;
    return node1->freq - node2->freq;
}

void huffman_tree_parse_to_table(Node *node, Huffman_Code *table, uint64_t code, int len) {
    if (!node) {
        return;
    }
    huffman_tree_parse_to_table(node->left, table, code << 1, len + 1);
    if (node_is_leaf(node)) {
        table[node->byte] = (Huffman_Code){
            .code = code,
            .len = len,
        };
    }
    huffman_tree_parse_to_table(node->right, table, (code << 1) | 1, len + 1);
}

void bitmap_append_huffman_tree(Bitmap *bitmap, Node *node) {
    if (!node) {
        return;
    }

    if (bitmap->items == NULL) {
        da_append(bitmap, 0);
    }

    if (node_is_leaf(node)) {
        bitmap_append_bit(bitmap, 1);
        bitmap_append_byte(bitmap, node->byte);
        return;
    }

    bitmap_append_bit(bitmap, 0);
    bitmap_append_huffman_tree(bitmap, node->left);
    bitmap_append_huffman_tree(bitmap, node->right);
}

void bitmap_append_bit(Bitmap *bitmap, uint8_t bit) {
    if (bitmap->count_bits == 8) {
        bitmap->count_bits = 0;
        da_append(bitmap, 0);
    }
    bitmap->items[bitmap->count - 1] |= (1 & bit) << (7 - bitmap->count_bits++);
}

void bitmap_append_byte(Bitmap *bitmap, uint8_t byte) {
    if (bitmap->count_bits == 0) {
        da_append(bitmap, byte);
        return;
    }

    bitmap->items[bitmap->count - 1] |= (byte >> bitmap->count_bits);
    da_append(bitmap, 0);
    bitmap->items[bitmap->count - 1] |= (byte << (8 - bitmap->count_bits));
}

void bitmap_append_huffman_code(Bitmap *bitmap, Huffman_Code hc) {
    int entire_bytes = hc.len / 8;
    int rest_bits = hc.len % 8;
    for (int i = rest_bits - 1; i >= 0; i--) {
        uint8_t byte = hc.code >> (8 * entire_bytes);
        uint8_t bit = byte >> i;
        bitmap_append_bit(bitmap, bit);
    }
    for (int i = entire_bytes - 1; i >= 0; i--) {
        uint8_t byte = hc.code >> (8 * i);
        bitmap_append_byte(bitmap, byte);
    }
}

void huffman_table_display(Huffman_Code *table) {
    for (int i = 0; i <= UINT8_MAX; i++) {
        Huffman_Code hc = table[i];
        if (hc.len == 0) {
            continue;
        }
        if (isascii(i)) {
            printf("%c => 0x%lx (len=%d)\n", i, hc.code, hc.len);
        } else {
            printf("0x%x => 0x%lx (len=%d)\n", i, hc.code, hc.len);
        }
    }
}
