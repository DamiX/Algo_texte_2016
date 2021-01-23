#include "include/queue.h"
#include <string.h>

const int DEFAULT_CAPACITY = 1 << 10;

void expand_queue(int_queue* q, const int new_capacity)
{
	int* new_values = (int*)malloc(sizeof(int) * new_capacity);
	if(new_values == NULL) {
		// fprintf(stderr, "%s: Allocation impossible", __FILE__);
		exit(1);
	}

	memcpy((void*)new_values, (void*)q->values, sizeof(int)*q->size);
	free(q->values);
	q->values = new_values;
	q->capacity = new_capacity;
}

int_queue init_queue()
{
	return init_queue_with_capacity(DEFAULT_CAPACITY);
}

int_queue init_queue_with_capacity(const int capacity)
{
	int_queue q;
	q.head		= 0;
	q.tail 		= 0;
	q.size 		= 0;
	q.capacity	= capacity;

	q.values = (int*)malloc(sizeof(int) * capacity);
	if(q.values == NULL) {
		// fprintf(stderr, "%s: Allocation impossible", __FILE__);
		exit(1);
	}

	return q;
}

void free_queue(int_queue* q)
{
	free(q->values);
	q->values = NULL;
}

int pop_queue(int_queue* q)
{
	int v = q->values[q->head];
	q->size -= 1;
	if(q->head == q->capacity)
		q->head = 1;
	else
		q->head += 1;

	return v;
}

void push_queue(const int x, int_queue* q)
{
	if(q->size == q->capacity) {
		const int new_capacity = q->capacity << 1;
		expand_queue(q, new_capacity);
	}

	q->values[q->tail] = x;
	q->size += 1;
	if(q->tail == q->capacity-1)
		q->tail = 1;
	else
		q->tail += 1;
}

