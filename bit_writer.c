#include <stdio.h>
#include <assert.h>
#include "bit_writer.h" 

static void _resetBitWriter(BitWriter *writer) 
{
	writer -> currByte = 0;
	writer -> numBitsLeft = 8;
}

static int _calcNumToShift(int numBitsToWrite, int numBitsLeft) 
{
	int result = numBitsToWrite - numBitsLeft;

	return result >= 0 ? result : -result;
}

BitWriter openBitWriter(const char *path)
{
	return (BitWriter) { .file = fopen(path, "wb"), .currByte = 0, .numBitsLeft = 8 };
}

/* numBitsToWrite 개수의 비트를 최하위 비트부터 writer에 작성하고 꽉차면 파일에 작성 */
void writeBits(BitWriter *writer, uint8_t bits, uint8_t numBitsToWrite)
{
	assert(numBitsToWrite >= 0 && numBitsToWrite <= 8);
	assert(writer -> numBitsLeft >= 1 && writer -> numBitsLeft <= 8);
	
	uint8_t mask = (1 << numBitsToWrite) - 1;
	uint8_t extractedBits = mask & bits;

	if (numBitsToWrite <= writer -> numBitsLeft) {
		extractedBits <<= _calcNumToShift(numBitsToWrite, writer -> numBitsLeft);
		writer -> currByte |= extractedBits;

		if ((writer -> numBitsLeft -= numBitsToWrite) == 0) {
			fwrite(&(writer -> currByte), sizeof(writer -> currByte), 1, writer -> file);
			_resetBitWriter(writer);
		}
	} else {
		uint8_t currByte = extractedBits >> _calcNumToShift(numBitsToWrite, writer -> numBitsLeft);
		writer -> currByte |= currByte;
		fwrite(&(writer -> currByte), sizeof(writer -> currByte), 1, writer -> file);
		uint8_t saveNumBitsLeft = writer -> numBitsLeft;
		_resetBitWriter(writer);

		uint8_t remainingNumBitsToWrite = numBitsToWrite - saveNumBitsLeft;

		mask = (1 << remainingNumBitsToWrite) - 1; 
		extractedBits = mask & bits;
		uint8_t nextByte = extractedBits << _calcNumToShift(remainingNumBitsToWrite, writer -> numBitsLeft);
		writer -> currByte |= nextByte;
		writer -> numBitsLeft -=  remainingNumBitsToWrite;
	}

	assert(writer -> numBitsLeft >= 1 && writer -> numBitsLeft <= 8);
}

/* 남은 비트를 파일에 작성 */
void flushBitWriter(BitWriter *writer)
{
	fwrite(&(writer -> currByte), sizeof(writer -> currByte), 1, writer -> file);
	_resetBitWriter(writer);
}

void closeBitWriter(BitWriter *writer)
{
	flushBitWriter(writer);
	fclose(writer -> file);
	writer -> file = NULL;
}