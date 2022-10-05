CC=gcc
CFLAGS=-g -Wall -Wshadow -Wvla -pedantic
DEBUG=gdb
EXECUTABLE=huff
MAIN_C=huffman_main.c
SRC_C=huffman.c frequency.c priority_queue.c
SRC_H=huffman.h frequency.h priority_queue.h

$(EXECUTABLE):$(MAIN_C) $(SRC_C) $(SRC_H) 
	$(CC) -o huff $(CFLAGS) huffman.c frequency.c priority_queue.c huffman_main.c

debug:
	$(DEBUG) huff

clean:
	rm .\$(EXECUTABLE)
