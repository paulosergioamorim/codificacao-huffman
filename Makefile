CC = gcc
CFLAGS = -Wall -Wextra
RELEASE_FLAGS = -O2
DEBUG_FLAGS = -g -O0 -DDEBUG
SRC = ./src

COMMON_SOURCES = $(SRC)/tree.c $(SRC)/bitreader.c
ENCODER_SOURCES = $(SRC)/heap.c $(SRC)/arraybyte.c $(SRC)/huffman.c

# Default build (release)
all: encoder decoder

# Release builds
encoder: $(SRC)/encoder.c $(COMMON_SOURCES) $(ENCODER_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(RELEASE_FLAGS)

decoder: $(SRC)/decoder.c $(COMMON_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(RELEASE_FLAGS)

# Debug builds
debug: CFLAGS += $(DEBUG_FLAGS)
debug: encoder_debug decoder_debug

encoder_debug: $(SRC)/encoder.c $(COMMON_SOURCES) $(ENCODER_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS)
	@echo "Built debug version: encoder_debug"

decoder_debug: $(SRC)/decoder.c $(COMMON_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS)
	@echo "Built debug version: decoder_debug"

# Clean
clean:
	rm -f encoder decoder encoder_debug decoder_debug

.PHONY: all clean debug