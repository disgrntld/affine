#include <stdlib.h>
#include "stack.h"

stack_t *stack_init()
{
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	stack->head = NULL;
	stack->size = 0;
	return stack;
}

int stack_is_empty(stack_t *stack)
{
	return !stack->size;
}

void *stack_peek(stack_t *stack)
{
	return stack->head->data;
}

void stack_push(stack_t *stack, void *data)
{
	stack_node_t *stack_node = malloc(sizeof(stack_node_t));
	stack_node->data = data;
	stack_node->next = stack->head;

	stack->head = stack_node;
	++stack->size;
}

void *stack_pop(stack_t *stack)
{
	stack_node_t *stack_node = stack->head;
	stack->head = stack_node->next;
	--stack->size;

	void *result = stack_node->data;
	free(stack_node);
	return result;
}

void stack_free(stack_t *stack)
{
	while(!stack_is_empty(stack))
		stack_pop(stack);

	free(stack);
}

