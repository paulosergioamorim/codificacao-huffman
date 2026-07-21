#define NOB_IMPLEMENTATION
#include "../nob.h"
#include "file_header.h"
#include "node.h"

void print_help();

typedef struct {
    FILE *input_stream;
    off_t index_bytes;
    off_t file_size;
    uint8_t temp;
    uint8_t count_bits;
    uint8_t count_last_bits;
} Bitstream;

Node *bitstream_read_huffman_tree(Bitstream *bs);

uint8_t bitstream_read_bit(Bitstream *bs);

uint8_t bitstream_read_byte(Bitstream *bs);

int main(int argc, char **argv) {
    if (argc == 1) {
        print_help();
        return 1;
    }

    const char *path = argv[1];
    String_View ext_sv = SVLIT(".comp");
    String_View path_sv = sv_from_cstr(path);

    if (!sv_ends_with(path_sv, ext_sv)) {
        nob_log(ERROR, "Invalid file name");
        return 1;
    }

    FILE *input_stream = fopen(path, "r");
    if (input_stream == NULL) {
        nob_log(ERROR, "Failed to create input file stream");
        return 1;
    }

    struct stat st = {0};
    if (fstat(fileno(input_stream), &st) == -1) {
        nob_log(ERROR, "Failed to stat input file");
        fclose(input_stream);
        return 1;
    }

    nob_log(INFO, "Reading header file");
    File_Header header = {0};
    ssize_t items_read = fread(&header, sizeof(header), 1, input_stream);
    if (items_read == -1) {
        nob_log(ERROR, "Failed to read header file");
        fclose(input_stream);
        return 1;
    }

    if (!file_header_is_valid(header)) {
        nob_log(ERROR, "Invalid file format");
        fclose(input_stream);
        return 1;
    }

    sv_chop_suffix(&path_sv, ext_sv);
    const char *new_path = temp_sv_to_cstr(path_sv);
    FILE *output_stream = fopen(new_path, "w+");
    if (output_stream == NULL) {
        nob_log(ERROR, "Failed to create output stream");
        fclose(input_stream);
        return 1;
    }

    if (header.count_last_valid_bits == 0) {
        nob_log(INFO, "Closing streams");
        fclose(input_stream);
        fclose(output_stream);
        return 0;
    } // empty file

    Bitstream bs = {
        .input_stream = input_stream,
        .count_last_bits = header.count_last_valid_bits,
        .file_size = st.st_size,
        .index_bytes = sizeof(header),
    };
    fread(&bs.temp, sizeof(bs.temp), 1, input_stream);

    nob_log(INFO, "Reading Huffman Tree");
    Node *huffman_tree = bitstream_read_huffman_tree(&bs);
    Node *node = huffman_tree;

    nob_log(INFO, "Decoding file");
    while (bs.count_last_bits != 0) {
        uint8_t bit = bitstream_read_bit(&bs);

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

        if (node_is_leaf(huffman_tree)) {
            node = huffman_tree;
        } // unique byte file

        if (node_is_leaf(node)) {
            fwrite(&node->byte, sizeof(node->byte), 1, output_stream);
            node = huffman_tree;
        }
    }

    node_destroy(huffman_tree);
    nob_log(INFO, "Closing streams");
    fclose(input_stream);
    fclose(output_stream);
    return 0;
}

void print_help() {
    printf("USAGE:\n"
           "\t./descompacta <file>\n");
}

uint8_t bitstream_read_bit(Bitstream *bs) {
    if (bs->count_bits == 8) {
        bs->count_bits = 0;
        fread(&bs->temp, sizeof(bs->temp), 1, bs->input_stream);
        bs->index_bytes++;
    }
    uint8_t byte = bs->temp;
    uint8_t bit = 1 & (byte >> (7 - bs->count_bits++));
    if (bs->index_bytes == bs->file_size - 1) {
        bs->count_last_bits--;
    }
    return bit;
}

uint8_t bitstream_read_byte(Bitstream *bs) {
    if (bs->count_bits == 0) {
        uint8_t byte = bs->temp;
        fread(&bs->temp, sizeof(bs->temp), 1, bs->input_stream);
        bs->index_bytes++;
        return byte;
    }

    uint8_t byte = bs->temp << bs->count_bits;
    fread(&bs->temp, sizeof(bs->temp), 1, bs->input_stream);
    byte |= (bs->temp >> (8 - bs->count_bits));
    if (++bs->index_bytes == bs->file_size - 1) {
        bs->count_last_bits -= bs->count_bits;
    }
    return byte;
}

Node *bitstream_read_huffman_tree(Bitstream *bs) {
    uint8_t bit = bitstream_read_bit(bs);
    Node *node = malloc(sizeof(*node));
    memset(node, 0, sizeof(*node));

    if (bit == 0) {
        node->left = bitstream_read_huffman_tree(bs);
        node->right = bitstream_read_huffman_tree(bs);
        return node;
    }

    node->byte = bitstream_read_byte(bs);
    return node;
}
