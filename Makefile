CC = gcc
CFLAGS = -Wall -Wextra -lm
RELEASE_FLAGS = -O2
DEBUG_FLAGS = -g -O0 -DDEBUG
SRC = ./src

COMMON_SOURCES = $(SRC)/tree.c $(SRC)/bitmap.c $(SRC)/readbuffer.c $(SRC)/huffman.c $(SRC)/utils.c
COMPACTA_SOURCES = $(SRC)/heap.c $(COMMON_SOURCES)

all: compacta descompacta

compacta: $(SRC)/compacta.c $(COMPACTA_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(RELEASE_FLAGS)

descompacta: $(SRC)/descompacta.c $(COMMON_SOURCES)
	$(CC) $^ -o $@ $(CFLAGS) $(RELEASE_FLAGS)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

clean:
	rm -f compacta descompacta

.PHONY: all clean debug