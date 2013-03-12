#ifndef AFFINE_H
#define AFFINE_H

#include "stack.h"
#include "sco.h"

typedef struct affine_vector
{
	float x;
	float y;
	float z;
	float p;
} affine_vector_t;

typedef struct affine_matrix
{
	affine_vector_t v0;
	affine_vector_t v1;
	affine_vector_t v2;
	affine_vector_t v3;
} affine_matrix_t;

stack_t *affine_init();
void push_affine_matrix(stack_t *);
void identity(stack_t *);
void translate(stack_t *, float, float, float);
void rotate(stack_t *, float, float, float, float);
void scale(stack_t *, float, float, float);
affine_vector_t apply_affine_matrix(stack_t *, affine_vector_t *);
void place_object(sco_file_t *sco_file, char *name, stack_t *stack);
void pop_affine_matrix(stack_t *);
void affine_free(stack_t *);

#endif

