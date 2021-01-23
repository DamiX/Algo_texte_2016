#ifndef __H_QUEUE__
#define __H_QUEUE__

#include <stdbool.h>
#include <stdlib.h>

typedef struct _queue{
	int head;
	int tail;
	int size;
	int capacity;
	int* values;
} int_queue;

int_queue init_queue();
int_queue init_queue_with_capacity(const int capacity);
void free_queue(int_queue*);

int pop_queue(int_queue*);
void push_queue(const int, int_queue*);

static inline bool is_queue_empty(int_queue* q) {
	return (0 == q->size) ? true : false;
}

#endif
