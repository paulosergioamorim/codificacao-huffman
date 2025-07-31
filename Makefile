CC = gcc
CFLAGS = -Wall -Wextra -lm
RELEASE_FLAGS = -O2
DEBUG_FLAGS = -g -O0 -DDEBUG
SRC = ./src

COMMON_SOURCES = $(SRC)/tree.c $(SRC)/bitarray.c $(SRC)/readbuffer.c $(SRC)/huffman.c $(SRC)/heap.c $(SRC)/utils.c

all: compacta descompacta

compacta: $(SRC)/compacta.c $(COMMON_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(RELEASE_FLAGS)

descompacta: $(SRC)/descompacta.c $(COMMON_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(RELEASE_FLAGS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

clean:
	rm -f compacta descompacta

.PHONY: all clean debug