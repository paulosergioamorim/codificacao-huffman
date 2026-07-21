#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static const uint8_t magic_bytes[4] = {~'C', ~'O', ~'M', ~'P'};

typedef struct {
    uint8_t magic_bytes[4];
    uint8_t count_last_valid_bits;
} File_Header;

static inline bool file_header_is_valid(File_Header header) {
    return memcmp(header.magic_bytes, magic_bytes, sizeof(magic_bytes)) == 0;
}

#endif // FILE_HEADER_H
