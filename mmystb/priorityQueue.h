#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#define ADDRESS(element) (&((typeof(element)){element}))
#define DUP(value) memdup(ADDRESS(value), sizeof(value))

typedef struct {
	void **vec;
	int size;
	int (*cmp)(const void *, const void *);
} heap;

typedef heap priorityQueue;

priorityQueue newPriorityQueue(int (*cmp)(const void *, const void *));
void priorityQueuePush(priorityQueue *q, void *data);
void *priorityQueuePop(priorityQueue *q);
const void *priorityQueuePeek(const priorityQueue *q);
int priorityQueueGetSize(const priorityQueue *q);
bool priorityQueueIsEmpty(const priorityQueue *q);
void priorityQueueFree(priorityQueue *q, void (*freeData)(void *));

void *memdup(const void *mem, const int size);
void swap(void *a, void *b, const size_t size);

#ifdef PRIORITYQUEUE_IMPL
#include <stdlib.h>
#include <limits.h>

heap newHeap(int (*cmp)(const void *, const void *))
{
	heap h;
	h.size = 0;
	h.vec = NULL;
	h.cmp = cmp;
	return h;
}

int parent(int i)
{
	return i / 2;
}

int right(int i)
{
	return (i * 2) + 2;
}

int left(int i)
{
	return (i * 2) + 1;
}

void heapifyUp(heap *h, int i)
{
	if (i == 0)
		return;

	if (h->cmp(h->vec[i], h->vec[parent(i)]) > 0) {
		swap(&h->vec[i], &h->vec[parent(i)], sizeof(void*));
		heapifyUp(h, parent(i));
	}
}

void heapifyDown(heap *h, int i)
{
	int l = left(i);
	int r = right(i);

	int largest = i;

	if (l < h->size && h->cmp(h->vec[l], h->vec[i]) > 0)
		largest = l;

	if (r < h->size && h->cmp(h->vec[r], h->vec[largest]) > 0)
		largest = r;

	if (largest != i) {
		swap(&h->vec[i], &h->vec[largest], sizeof(void*));
		heapifyDown(h, largest);
	}
}

void heapPush(heap *h, void *data)
{
	h->vec = realloc(h->vec, sizeof(void*) * ++h->size);
	h->vec[h->size - 1] = data;
	heapifyUp(h, h->size - 1);
}

const void *heapPeek(const heap *h)
{
	return h->vec[0];
}

void *heapPop(heap *h)
{
	void *ret = h->vec[0];
	h->vec[0] = h->vec[h->size - 1];
	h->vec = realloc(h->vec, sizeof(void*) * --h->size);
	heapifyDown(h, 0);
	return ret;
}

int heapGetSize(const heap *h)
{
	return h->size;
}

bool heapIsEmpty(const heap *h)
{
	return heapGetSize(h) == 0;
}

void heapFree(heap *h, void (*freeData)(void *))
{
	if (freeData != NULL)
		for (int i = 0; i < h->size; i++)
			freeData(h->vec[i]);
	free(h->vec);
}

priorityQueue newPriorityQueue(int (*cmp)(const void *, const void *))
{
	return newHeap(cmp);
}

void priorityQueuePush(priorityQueue *q, void *data)
{
	heapPush(q, data);
}

void *priorityQueuePop(priorityQueue *q)
{
	return heapPop(q);
}

const void *priorityQueuePeek(const priorityQueue *q)
{
	return heapPeek(q);
}

int priorityQueueGetSize(const priorityQueue *q)
{
	return heapGetSize(q);
}

bool priorityQueueIsEmpty(const priorityQueue *q)
{
	return heapIsEmpty(q);
}

void priorityQueueFree(priorityQueue *q, void (*freeData)(void *))
{
	heapFree(q, freeData);
}

#ifndef MEMDUP
#define MEMDUP
void *memdup(const void *mem, const int size)
{
	void *newMem = malloc(size);
	memcpy(newMem, mem, size);
	return newMem;
}
#endif

#ifndef SWAP
#define SWAP
void swap(void *a, void *b, const size_t size)
{
	char tmp[size];
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
}
#endif

#endif
#endif
