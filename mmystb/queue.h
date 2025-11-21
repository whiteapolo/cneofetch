#ifndef QUEUE_H
#define QUEUE_H
#include "list.h"

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))

typedef struct {
    list *start;
    list *end;
} queue;

queue newQueue();
void enqueue(queue *q, void *data);
void *dequeue(queue *q);
bool queueIsEmpty(const queue *q);
void queueFree(queue *q, void (*freeData)(void *));
void queuePrint(const queue *q, void (*printData)(void *));

#ifdef QUEUE_IMPL
#include <string.h>

queue newQueue()
{
	queue q;
	q.start = NULL;
	q.end = NULL;
	return q;
}

void enqueue(queue *q, void *data)
{
	if (q->start == NULL) {
		listPush(&q->start, data);
		q->end = q->start;
	} else {
		listInsertAfter(q->end, data);
		q->end = q->end->next;
	}
}

void *dequeue(queue *q)
{
	list *tmp = q->start->next;
	void *data = listPop(&q->start);
	q->start = tmp;
	return data;
}

bool queueIsEmpty(const queue *q)
{
	return q->start == NULL;
}

void queuePrint(const queue *q, void (*printData)(void *))
{
	listPrint(q->start, printData);
}

void queueFree(queue *q, void (*freeData)(void *))
{
	if (freeData)
		while (!queueIsEmpty(q))
			freeData(dequeue(q));
}

#endif
#endif
