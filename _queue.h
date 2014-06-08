#ifndef __QUEUE_H__
#define __QUEUE_H__
#include"parser.h"

typedef struct _qnode {
	e_node* v;
	struct _qnode* next;
} qnode;

typedef struct _queue {
	qnode* head;
	qnode* rear;
} queue;

queue* init_queue();
void push_queue(e_node* v,queue* q);
int empty_queue(queue* q);
e_node* top_queue(queue* q);

#endif
