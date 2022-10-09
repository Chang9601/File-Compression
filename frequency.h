#ifndef __FREQUENCY_H__
#define __FREQUENCY_H__

#include <stdbool.h>
#include <stdint.h>

typedef unsigned char uchar;
bool calcFrequencies(uint64_t freq[256], const char *path, const char **error);

#endif
