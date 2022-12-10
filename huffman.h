#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <stdlib.h>
#include "frequency.h"
#include "priority_queue.h"
#include "bit_writer.h"

typedef struct _HuffTreeNode {
	uchar ch;
	size_t freq;
	struct _HuffTreeNode *left;
	struct _HuffTreeNode *right;
} HuffTreeNode;

typedef struct _EncodedCharacter {
	uchar ch[256];
	size_t len[256];	
} EncodedCharacter;

Node *makeHuffmanPq(uint64_t freqs[256]);
HuffTreeNode *makeHuffmanTree(Node *pq);
void destroyHuffmanTree(HuffTreeNode **root);
void writeCompressedBytes(HuffTreeNode *root, BitWriter *writer, uint8_t *rawBytes);

#endif