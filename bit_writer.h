#ifndef __BIT_WRITER_H__
#define __BIT_WRITER_H__

#include <stdio.h>
#include <stdint.h>

typedef struct _BitWriter {
	FILE *file;
	uint8_t currByte;
	uint8_t numBitsLeft;
} BitWriter;

BitWriter openBitWriter(const char *path);
void writeBits(BitWriter *writer, uint8_t bits, uint8_t numBitsToWrite);
void flushBitWriter(BitWriter *writer);
void closeBitWriter(BitWriter *writer);

#endif