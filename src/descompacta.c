#define NOB_IMPLEMENTATION
#include "../nob.h"
#include "tree.h"
#include <fcntl.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>

void print_help();

typedef struct bit_reader {
    uint8_t *buf;
    off_t size;
    int index_bytes;
    int count_bits;
} Bit_Reader;

Node *bitreader_read_huffman_tree(Bit_Reader *br);

uint8_t bitreader_read_bit(Bit_Reader *br);

uint8_t bitreader_read_byte(Bit_Reader *br);

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

    uint8_t *buf = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (buf == MAP_FAILED) {
        printf("Failed to allocate buffer\n");
        close(fd);
        return 1;
    }

    if (madvise(buf, st.st_size, MADV_HUGEPAGE) == -1) {
        printf("Failed to use transparent huge pages\n");
    }

    Bit_Reader br = {
        .buf = buf,
        .size = st.st_size,
    };

    uint8_t count_last_bits = bitreader_read_byte(&br);
    Node *huffman_tree = bitreader_read_huffman_tree(&br);
    Node *node = huffman_tree;

    sv_chop_suffix(&path_sv, ext_sv);

    const char *new_path = temp_sv_to_cstr(path_sv);
    FILE *fp = fopen(new_path, "w+");

    if (fp == NULL) {
        printf("Failed to open output file\n");
        munmap(buf, st.st_size);
        close(fd);
        node_destroy(huffman_tree);
        return 0;
    }

    while (count_last_bits != 0) {
        uint8_t bit = bitreader_read_bit(&br);

        if (br.index_bytes == br.size - 1) {
            count_last_bits--;
        }

        switch (bit) {
        case 0:
            node = node->left;
            break;
        case 1:
            node = node->right;
            break;
        default:
            NOB_UNREACHABLE("Invalid bit");
        }

        if (node_is_leaf(node)) {
            fwrite(&node->byte, sizeof(node->byte), 1, fp);
            node = huffman_tree;
        }
    }

    if (munmap(buf, st.st_size) == -1) {
        printf("Failed to munmap buffer\n");
    }

    if (close(fd) == -1) {
        printf("Failed to close compressed file\n");
        perror(NULL);
    }

    node_destroy(huffman_tree);
    fclose(fp);
    return 0;
}

void print_help() {
    printf("./descompacta <file>");
}

uint8_t bitreader_read_bit(Bit_Reader *br) {
    if (br->count_bits == 8) {
        br->count_bits = 0;
        br->index_bytes++;
    }
    uint8_t byte = br->buf[br->index_bytes];
    uint8_t bit = 1 & (byte >> (7 - br->count_bits++));
    return bit;
}

uint8_t bitreader_read_byte(Bit_Reader *br) {
    if (br->count_bits == 0) {
        uint8_t byte = br->buf[br->index_bytes++];
        return byte;
    }

    uint8_t byte = br->buf[br->index_bytes++] << br->count_bits;
    byte |= (br->buf[br->index_bytes] >> (8 - br->count_bits));
    return byte;
}

Node *bitreader_read_huffman_tree(Bit_Reader *br) {
    uint8_t bit = bitreader_read_bit(br);
    Node *node = malloc(sizeof(*node));
    memset(node, 0, sizeof(*node));

    if (bit == 0) {
        node->left = bitreader_read_huffman_tree(br);
        node->right = bitreader_read_huffman_tree(br);
        return node;
    }

    node->byte = bitreader_read_byte(br);
    return node;
}
