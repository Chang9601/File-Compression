#ifndef __PRIORITY_QUEUE_H__
#define __PRIORITY_QUEUE_H__

typedef struct _Node {
	void *value;
	struct _Node *next;
} Node;

Node *enqueue(Node **pq, void *value, int (*cmpFunc)(const void*, const void*));
Node *dequeue(Node **pq);
void destroyList(Node **head, void (*destroyFunc)(void*));

#endif
