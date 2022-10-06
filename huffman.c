#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huffman.h"

static int _cmpFrequencies(const void *argLhs, const void *argRhs);

// Create a priority queue for Huffman tree.
Node *makeHuffmanPq(uint64_t freqs[256])
{
	Node *head = NULL;

	for (uchar ch = 0; ch < 256; ch++) {
		if (freqs[ch] != 0) {
			HuffTreeNode *huffTreeNode = calloc(1, sizeof(*huffTreeNode));
			huffTreeNode -> ch = ch;
			huffTreeNode -> freq = freqs[ch];
			huffTreeNode -> left = huffTreeNode -> right = NULL;	

			enqueue(&head, huffTreeNode, _cmpFrequencies);
		}	

		if (ch == 255)
			break;
	}

	return head;
}

// Create a Huffman tree from priority queue.
HuffTreeNode *makeHuffmanTree(Node *pq)
{
	if (pq == NULL)
		return NULL;
	
	while (pq -> next != NULL) {
		// Remove the first two nodes from priority queue.
		Node *firstNode = dequeue(&pq);
		Node *secondNode = dequeue(&pq);

		// Extract Huffman tree nodes.
		HuffTreeNode *firstHuffTreeNode = (HuffTreeNode*) (firstNode -> value);
		HuffTreeNode *secondHuffTreeNode = (HuffTreeNode*) (secondNode -> value);

		// Combine two Huffman tree nodes into a new one.
		HuffTreeNode *huffTreeNode = calloc(1, sizeof(*huffTreeNode));
		huffTreeNode -> ch = '\0';
		huffTreeNode -> freq = firstHuffTreeNode -> freq + secondHuffTreeNode -> freq;
		huffTreeNode -> left = firstHuffTreeNode;
		huffTreeNode -> right = secondHuffTreeNode;

		// Put the newly created huffman tree node into the priority queue.
		enqueue(&pq, huffTreeNode, _cmpFrequencies);

		// Deallocate memory nodes of the priority queue.
		free(firstNode);
		free(secondNode);
	}

	// Extract a completed Huffam tree from the priority queue.
	HuffTreeNode *root = (HuffTreeNode*) (pq -> value);
	free(pq);

	return root;
}

// Destroy a Huffman tree.
void destroyHuffmanTree(HuffTreeNode **root)
{
	if ((*root) != NULL) {
		destroyHuffmanTree(&((*root) -> left));
		destroyHuffmanTree(&((*root) -> right));
		free(*root);
	}

	*root = NULL;
}

static int _cmpFrequencies(const void *argLhs, const void *argRhs) 
{
	const HuffTreeNode *huffTreeNodeLhs = (HuffTreeNode*)argLhs;
	const HuffTreeNode *huffTreeNodeRhs = (HuffTreeNode*)argRhs;

	if (huffTreeNodeLhs -> freq < huffTreeNodeRhs -> freq) 
		return -1;
	else if (huffTreeNodeLhs -> freq > huffTreeNodeRhs -> freq) 
		return 1;
	else {
		if (huffTreeNodeLhs -> ch == '\0')
			return 1;	
		else if (huffTreeNodeRhs -> ch == '\0') 
			return -1;
		else 
			return (huffTreeNodeLhs -> ch) - (huffTreeNodeRhs -> ch);
	}
}
