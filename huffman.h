#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdlib.h>
#include "frequency.h"
#include "priority_queue.h"

typedef struct _HuffTreeNode {
	uchar ch;
	size_t freq;
	struct _HuffTreeNode *left;
	struct _HuffTreeNode *right;
} HuffTreeNode;

Node *makeHuffmanPq(uint64_t freqs[256]);

#endif
