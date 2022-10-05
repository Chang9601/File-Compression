#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huffman.h"

static int _cmpFrequencies(const void *argLhs, const void *argRhs);

Node *makeHuffmanPq(uint64_t freqs[256])
{
	Node *head = NULL;

	for (uchar ch = 0; ch < 256; ch++) {
		if (freqs[ch] != 0) {
			HuffTreeNode *treeNode = calloc(1, sizeof(*treeNode));
			treeNode -> ch = ch;
			treeNode -> freq = freqs[ch];
			treeNode -> left = treeNode -> right = NULL;	

			enqueue(&head, treeNode, _cmpFrequencies);
		}	

		if (ch == 255)
			break;
	}

	return head;
}

static int _cmpFrequencies(const void *argLhs, const void *argRhs) 
{
	const HuffTreeNode *treeNodeLhs = (HuffTreeNode*)argLhs;
	const HuffTreeNode *treeNodeRhs = (HuffTreeNode*)argRhs;

	if (treeNodeLhs -> freq < treeNodeRhs -> freq) 
		return -1;
	else if (treeNodeLhs -> freq > treeNodeRhs -> freq) 
		return 1;
	else 
		return (treeNodeLhs -> ch) - (treeNodeRhs -> ch);
}
