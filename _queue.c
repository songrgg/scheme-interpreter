#include<stdio.h>
#include"_memory.h"
#include"parser.h"
#include"_queue.h"

queue* init_queue(void)
{
	queue* ret = (queue*)_alloc(sizeof(queue));
	ret->head = ret->rear = NULL;
	return ret;
}

void push_queue(e_node* v,queue* q)
{
	if (q->head == q->rear && q->head == NULL) {
		q->head = q->rear = (qnode*)_alloc(sizeof(qnode));
		q->head->v = v;
	}
	else {
		q->rear->next = (qnode*)_alloc(sizeof(qnode));
		q->rear->next->v = v;
		q->rear = q->rear->next;
	}
}

int empty_queue(queue* q)
{
	return (q->head == q->rear && q->head == NULL);
}

e_node* top_queue(queue* q)
{
	if (empty_queue(q)) {
		return NULL;
	}
	else if (q->head == q->rear) {
		e_node* ret = q->head->v;
		_dealloc(q->head);
		q->head = q->rear = NULL;
		return ret;
	}
	else {
		e_node* ret = q->head->v;
		_dealloc(q->head);
		q->head = q->head->next;
		return ret;
	}
}
