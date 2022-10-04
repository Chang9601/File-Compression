#include <stdlib.h>
#include "priority_queue.h"

Node *enqueue(Node **pq, void *value, int (*cmpFunc)(const void*, const void*)) 
{
	Node *node = calloc(1, sizeof(*node));
	node -> value = value;
	node -> next = NULL;

	if (*pq == NULL || cmpFunc(node -> value, (*pq) -> value) < 0) {
		node -> next = *pq;
		*pq = node;
	} else {
		Node *curr, *prev;
		prev = curr = *pq;

		while (curr != NULL && cmpFunc(node -> value, curr -> value) >= 0) {
			prev = curr;
			curr = curr -> next;
		}

		prev -> next = node;
		node -> next = curr;
	}

	return node;
}

Node *dequeue(Node **pq) 
{
	Node *node = *pq;
	if (node !=  NULL) {
		*pq = (*pq) -> next;
		node -> next = NULL;
	}

	return node;	
}

void destroyList(Node **head, void (*destroyFunc)(void*)) 
{
	while (*head != NULL) {
		Node *node = (*head) -> next;
		destroyFunc((*head) -> value);
		free(*head);
		*head = node;
	}

	*head = NULL;
}
