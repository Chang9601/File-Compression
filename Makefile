CC=gcc
CFLAGS=-g -Wall -Wshadow -Wvla -Werror -pedantic
DEBUG=gdb
SRC_H=priority_queue.h frequency.h bit_writer.h huffman.h
SRC_C=priority_queue.c frequency.c bit_writer.c huffman.c main.c
EXECUTABLE=huffman

$(EXECUTABLE):$(SRC_H) $(SRC_C)
	$(CC) -o huffman $(CFLAGS) priority_queue.c frequency.c bit_writer.c huffman.c main.c

debug:
	$(DEBUG) huffman

clean:
	rm ./$(EXECUTABLE)
