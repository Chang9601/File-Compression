CC=gcc
CFLAGS=-g -Wall -Wshadow -Werror -Wvla -pedantic
DEBUG=gdb
EXECUTABLE=pq
MAIN_C=priority_queue_main.c
SRC_C=priority_queue.c
SRC_H=priority_queue.h

$(EXECUTABLE):$(MAIN_C) $(SRC_C) $(SRC_H) 
	$(CC) -o pq $(CFLAGS) priority_queue.c priority_queue_main.c

debug:
	$(DEBUG) pq

clean:
	rm .\$(EXECUTABLE)
