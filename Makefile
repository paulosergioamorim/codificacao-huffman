CC=gcc
CFLAGS=-Wall -lm
CGFLAGS=-g
OUTPUT=prog
SRC=./src
TARGET=$(SRC)/*.c

run: $(TARGET)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(TARGET)

debug: $(TARGET)
	$(CC) $(CFLAGS) $(CGFLAGS) -o $(OUTPUT) $(TARGET)