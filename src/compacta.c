#define NOB_IMPLEMENTATION
#include "../nob.h"
#include "file_header.h"
#include "node.h"

void print_help();

typedef struct {
    int count;
    int capacity;
    Node **items;
} Nodes;

typedef struct {
    uint64_t code;
    int len;
} Huffman_Code;

typedef struct {
    FILE *output_stream;
    uint8_t temp;
    uint8_t count_bits;
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
    FILE *input_stream = fopen(path, "r");
    uint8_t input_byte = 0;

    if (input_stream == NULL) {
        nob_log(ERROR, "Failed to create input file stream");
        return 1;
    }

    String_Builder sb = {0};
    sb_append_cstr(&sb, path);
    sb_append_cstr(&sb, ".comp");
    sb_append_null(&sb);

    File_Header header = {0};
    memcpy(header.magic_bytes, magic_bytes, sizeof(magic_bytes));
    FILE *output_stream = fopen(sb.items, "w+");
    sb_free(sb);

    size_t items_written = fwrite(&header, sizeof(header), 1, output_stream);
    if (items_written == 0) {
        nob_log(ERROR, "Failed to write header file");
    }

    if (output_stream == NULL) {
        nob_log(ERROR, "Failed to create output file stream");
        fclose(input_stream);
        return 1;
    }

    off_t freq[UINT8_MAX + 1] = {0};

    nob_log(INFO, "Counting byte frequencies");

    while (fread(&input_byte, sizeof(input_byte), 1, input_stream) > 0) {
        freq[input_byte]++;
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

    if (nodes.count == 0) {
        nob_log(INFO, "Closing streams");
        fclose(input_stream);
        fclose(output_stream);
        return 0;
    } // empty file

    nob_log(INFO, "Building Huffman Tree");
    while (nodes.count != 1) {
        qsort(nodes.items, nodes.count, sizeof(*nodes.items), node_compare);
        Node *node = malloc(sizeof(*node));
        if (node == NULL) {
            nob_log(ERROR, "Failed to malloc node");
            exit(1);
        }
        node->byte = 0;
        node->freq = nodes.items[0]->freq + nodes.items[1]->freq;
        node->left = nodes.items[0];
        node->right = nodes.items[1];
        memmove(nodes.items, nodes.items + 2, (nodes.count - 2) * sizeof(*nodes.items));
        nodes.count -= 2;
        da_append(&nodes, node);
    }

    Node *huffman_tree = nodes.items[0];
    da_free(nodes);

    nob_log(INFO, "Encoding file");
    Bitmap bitmap = {.output_stream = output_stream};
    bitmap_append_huffman_tree(&bitmap, huffman_tree);

    Huffman_Code table[UINT8_MAX + 1] = {0};
    huffman_tree_parse_to_table(huffman_tree, table, 0, 0);
    node_destroy(huffman_tree);

    if (fseeko(input_stream, 0, SEEK_SET) == -1) {
        nob_log(ERROR, "Failed to seek file");
    }

    while (fread(&input_byte, sizeof(input_byte), 1, input_stream) > 0) {
        Huffman_Code huffman_code = table[input_byte];
        if (huffman_code.len == 0) {
            bitmap_append_bit(&bitmap, 0);
        } // unique byte file
        bitmap_append_huffman_code(&bitmap, huffman_code);
    }

    header.count_last_valid_bits = bitmap.count_bits;
    if (bitmap.count_bits > 0) {
        size_t items_written = fwrite(&bitmap.temp, sizeof(bitmap.temp), 1, output_stream);
        if (items_written == 0) {
            nob_log(ERROR, "Failed to write file");
        }
    }

    if (fseeko(output_stream, 0, SEEK_SET) == -1) {
        nob_log(ERROR, "Failed to seek file");
    }

    items_written = fwrite(&header, sizeof(header), 1, output_stream);
    if (items_written == 0) {
        nob_log(ERROR, "Failed to write header file");
    }

    nob_log(INFO, "Closing streams");
    fclose(input_stream);
    fclose(output_stream);
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
        fwrite(&bitmap->temp, sizeof(bitmap->temp), 1, bitmap->output_stream);
        bitmap->temp = 0;
    }
    bitmap->temp |= (1 & bit) << (7 - bitmap->count_bits++);
}

void bitmap_append_byte(Bitmap *bitmap, uint8_t byte) {
    if (bitmap->count_bits == 0) {
        fwrite(&byte, sizeof(byte), 1, bitmap->output_stream);
        return;
    }

    bitmap->temp |= (byte >> bitmap->count_bits);
    fwrite(&bitmap->temp, sizeof(bitmap->temp), 1, bitmap->output_stream);
    bitmap->temp = 0;
    bitmap->temp |= (byte << (8 - bitmap->count_bits));
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
