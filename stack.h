#ifndef STACK_H
#define STACK_H

typedef struct stack_node {
	void *data;
	struct stack_node *next;
} stack_node_t;

typedef struct stack {
	stack_node_t *head;
	int size;
} stack_t;

stack_t *stack_init();
int stack_is_empty(stack_t *);
void *stack_peek(stack_t *);
void stack_push(stack_t *, void *);
void *stack_pop(stack_t *);
void stack_free(stack_t *);

#endif

