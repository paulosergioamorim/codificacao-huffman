CC=gcc
CFLAGS=-Wall -lm
CGFLAGS=-g
SRC=./src
TARGET_ENCODER=$(SRC)/encoder.c $(SRC)/heap.c $(SRC)/tree.c $(SRC)/arraybyte.c $(SRC)/bitreader.c $(SRC)/huffman.c
TARGET_DECODER=$(SRC)/decoder.c $(SRC)/tree.c $(SRC)/arraybyte.c $(SRC)/bitreader.c $(SRC)/huffman.c

encoder: $(TARGET_ENCODER)
	$(CC) $(TARGET_ENCODER) -o encoder $(CFLAGS) $(CGFLAGS)

decoder: $(TARGET_DECODER)
	$(CC) $(TARGET_DECODER) -o decoder $(CFLAGS) 

debug_decoder: $(TARGET_DECODER)
	$(CC) $(TARGET_DECODER) -o decoder $(CFLAGS) $(CGFLAGS)
