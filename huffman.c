#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huffman.h"

static int _cmpHuffmanTrees(const void *argLhs, const void *argRhs);
static void _makeEncodingTable(HuffTreeNode *root, EncodedCharacter *encodedCh, uchar ch, size_t len);

// Create a priority queue for a Huffman tree.
Node *makeHuffmanPq(uint64_t freqs[256])
{
	Node *head = NULL;

	for (uchar ch = 0; ch < 256; ch++) {
		if (freqs[ch] != 0) {
			HuffTreeNode *huffTreeNode = calloc(1, sizeof(*huffTreeNode));
			huffTreeNode -> ch = ch;
			huffTreeNode -> freq = freqs[ch];
			huffTreeNode -> left = huffTreeNode -> right = NULL;	

			enqueue(&head, huffTreeNode, _cmpHuffmanTrees);
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

		// Put the newly created Huffman tree node into the priority queue.
		enqueue(&pq, huffTreeNode, _cmpHuffmanTrees);

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

// Compress bits with a Huffman tree and write them to a file.
void writeCompressedBytes(HuffTreeNode *root, BitWriter *writer, uint8_t *rawBytes) 
{
	EncodedCharacter encodedCh;
	_makeEncodingTable(root, &encodedCh, 0x00, 0);

	for (uint8_t *ch = rawBytes; *ch != '\0'; ch++)
		writeBits(writer, encodedCh.ch[*ch], encodedCh.len[*ch]);
}

// Compare two Huffman trees.
static int _cmpHuffmanTrees(const void *argLhs, const void *argRhs) 
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

// Make an encoding table from a Huffman tree.
static void _makeEncodingTable(HuffTreeNode *root, EncodedCharacter *encodedCh, uchar ch, size_t len)
{
	if (root!= NULL && root -> ch != '\0') {
		encodedCh -> ch[root -> ch] = ch;
		encodedCh -> len[root -> ch] = len;
	}

	// Left subtree	
	if (root -> left != NULL) 
		_makeEncodingTable(root -> left, encodedCh, ch << 1, len + 1);

	// Right subtree	
	if (root -> right != NULL) 
		_makeEncodingTable(root -> right, encodedCh, (ch << 1) | 0x01, len + 1);
}
