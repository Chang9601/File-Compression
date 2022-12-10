#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "frequency.h"

/* 열린 파일에서 문자들의 빈도 계산 */
bool calcFrequencies(uint64_t freq[256], const char *path, const char **error)
{
	FILE *fp;

	if ((fp = fopen(path, "r")) == NULL) {
		*error = strerror(errno);
		return false;
	}
	
	for (uchar ch = getc(fp); !feof(fp); ch = getc(fp))
		freq[ch]++;

	fclose(fp);
	return true;	
}