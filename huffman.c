#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "huffman.h"

static int _cmpHuffmanTrees(const void *argLhs, const void *argRhs);
static void _makeEncodingTable(HuffTreeNode *root, EncodedCharacter *encodedCh, uchar ch, size_t len);

/* 허프만 트리를 위한 우선순위 큐 생성 */
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

/* 우선순위 큐로부터 허프만 트리 생성 */
HuffTreeNode *makeHuffmanTree(Node *pq)
{
	if (pq == NULL)
		return NULL;
	
	while (pq -> next != NULL) {
		/* 우선순위 큐에서 처음 2개의 노드 제거 */
		Node *firstNode = dequeue(&pq);
		Node *secondNode = dequeue(&pq);

		/* 허프만 트리 노드 추출 */
		HuffTreeNode *firstHuffTreeNode = (HuffTreeNode*) (firstNode -> value);
		HuffTreeNode *secondHuffTreeNode = (HuffTreeNode*) (secondNode -> value);

		/* 두 개의 허프만 트리 노드를 합쳐서 새로운 허프만 트리 노드 생성 */
		HuffTreeNode *huffTreeNode = calloc(1, sizeof(*huffTreeNode));
		huffTreeNode -> ch = '\0';
		huffTreeNode -> freq = firstHuffTreeNode -> freq + secondHuffTreeNode -> freq;
		huffTreeNode -> left = firstHuffTreeNode;
		huffTreeNode -> right = secondHuffTreeNode;

		/* 새로 생성된 허프만 트리 노드를 우선순위 큐에 삽입 */
		enqueue(&pq, huffTreeNode, _cmpHuffmanTrees);

		/* 우선순위 큐의 메모리 노드 해제 */
		free(firstNode);
		free(secondNode);
	}

	/* 우선순위 큐에서 완성된 허프만 트리 추출 */
	HuffTreeNode *root = (HuffTreeNode*) (pq -> value);
	free(pq);

	return root;
}

/* 허프만 트리 파괴 */
void destroyHuffmanTree(HuffTreeNode **root)
{
	if ((*root) != NULL) {
		destroyHuffmanTree(&((*root) -> left));
		destroyHuffmanTree(&((*root) -> right));
		free(*root);
	}

	*root = NULL;
}

/* 허프만 트리로 비트를 압축하고 파일에 작성 */
void writeCompressedBytes(HuffTreeNode *root, BitWriter *writer, uint8_t *rawBytes) 
{
	EncodedCharacter encodedCh;
	_makeEncodingTable(root, &encodedCh, 0x00, 0);

	for (uint8_t *ch = rawBytes; *ch != '\0'; ch++)
		writeBits(writer, encodedCh.ch[*ch], encodedCh.len[*ch]);
}

/* 2개의 허프만 트리 비교 */
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

/* 허프만 트리에서 부호화 테이블 생성 */
static void _makeEncodingTable(HuffTreeNode *root, EncodedCharacter *encodedCh, uchar ch, size_t len)
{
	if (root!= NULL && root -> ch != '\0') {
		encodedCh -> ch[root -> ch] = ch;
		encodedCh -> len[root -> ch] = len;
	}

	/* 왼쪽 서브트리 */	
	if (root -> left != NULL) 
		_makeEncodingTable(root -> left, encodedCh, ch << 1, len + 1);

	/* 오른쪽 서브트리 */	
	if (root -> right != NULL) 
		_makeEncodingTable(root -> right, encodedCh, (ch << 1) | 0x01, len + 1);
}