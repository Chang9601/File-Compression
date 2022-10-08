#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "frequency.h"

// Calculate frequency of characters in the open file.
bool calcFrequencies(uint64_t freqs[256], const char *path, const char **error)
{
	FILE *fp;

	if ((fp = fopen(path, "r")) == NULL) {
		*error = strerror(errno);
		return false;
	}
	
	for (uchar ch = getc(fp); !feof(fp); ch = getc(fp))
		freqs[ch]++;

	fclose(fp);
	return true;	
}
