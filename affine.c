#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "affine.h"

typedef union
{
	vector_t vector;
	affine_vector_t affine_vector;
} vector_wrapper_t;

void print_vector(vector_t *vector)
{
	printf("{%f, %f, %f}", vector->x, vector->y, vector->z);
}

void print_matrix(matrix_t *matrix)
{
	printf("[");
	print_vector(&matrix->v0);
	printf(", ");
	print_vector(&matrix->v1);
	printf(", ");
	print_vector(&matrix->v2);
	printf("] ");
	print_vector(&matrix->o);
}

void print_mission_object(mission_object_t *mission_object)
{
	printf("%s: meta_type = %d, sub_kind_no = %d, variation_id = %d, variation_id_2 = %d, position = ",
			mission_object->id,
			mission_object->meta_type,
			mission_object->sub_kind_no,
			mission_object->variation_id,
			mission_object->variation_id_2);
	print_matrix(&mission_object->position);
	printf(", scale = ");
	print_vector(&mission_object->scale);
}

affine_matrix_t *build_affine_matrix(
		float v0x, float v0y, float v0z, float v0p,
		float v1x, float v1y, float v1z, float v1p,
		float v2x, float v2y, float v2z, float v2p,
		float v3x, float v3y, float v3z, float v3p)
{
	affine_matrix_t *result = malloc(sizeof(affine_matrix_t));
	if(!result)
	{
		printf("ERROR: out of memory\n");
		return NULL;
	}
	affine_matrix_t temp = {
			{v0x, v0y, v0z, v0p},
			{v1x, v1y, v1z, v1p},
			{v2x, v2y, v2z, v2p},
			{v3x, v3y, v3z, v3p}};
	*result = temp;

	return result;
}

void affine_matrix_multiply(affine_matrix_t *left, affine_matrix_t *right)
{
	affine_matrix_t result = {
			{
					left->v0.x * right->v0.x + left->v0.y * right->v1.x + left->v0.z * right->v2.x + left->v0.p * right->v3.x,
					left->v0.x * right->v0.y + left->v0.y * right->v1.y + left->v0.z * right->v2.y + left->v0.p * right->v3.y,
					left->v0.x * right->v0.z + left->v0.y * right->v1.z + left->v0.z * right->v2.z + left->v0.p * right->v3.z,
					left->v0.x * right->v0.p + left->v0.y * right->v1.p + left->v0.z * right->v2.p + left->v0.p * right->v3.p},
			{
					left->v1.x * right->v0.x + left->v1.y * right->v1.x + left->v1.z * right->v2.x + left->v1.p * right->v3.x,
					left->v1.x * right->v0.y + left->v1.y * right->v1.y + left->v1.z * right->v2.y + left->v1.p * right->v3.y,
					left->v1.x * right->v0.z + left->v1.y * right->v1.z + left->v1.z * right->v2.z + left->v1.p * right->v3.z,
					left->v1.x * right->v0.p + left->v1.y * right->v1.p + left->v1.z * right->v2.p + left->v1.p * right->v3.p},
			{
					left->v2.x * right->v0.x + left->v2.y * right->v1.x + left->v2.z * right->v2.x + left->v2.p * right->v3.x,
					left->v2.x * right->v0.y + left->v2.y * right->v1.y + left->v2.z * right->v2.y + left->v2.p * right->v3.y,
					left->v2.x * right->v0.z + left->v2.y * right->v1.z + left->v2.z * right->v2.z + left->v2.p * right->v3.z,
					left->v2.x * right->v0.p + left->v2.y * right->v1.p + left->v2.z * right->v2.p + left->v2.p * right->v3.p},
			{
					left->v3.x * right->v0.x + left->v3.y * right->v1.x + left->v3.z * right->v2.x + left->v3.p * right->v3.x,
					left->v3.x * right->v0.y + left->v3.y * right->v1.y + left->v3.z * right->v2.y + left->v3.p * right->v3.y,
					left->v3.x * right->v0.z + left->v3.y * right->v1.z + left->v3.z * right->v2.z + left->v3.p * right->v3.z,
					left->v3.x * right->v0.p + left->v3.y * right->v1.p + left->v3.z * right->v2.p + left->v3.p * right->v3.p}};

	*left = result;
}

float vector_length(vector_t *vector)
{
	return sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

void normalize_vector(vector_t *vector)
{
	float length = vector_length(vector);
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}

stack_t *affine_init()
{
	stack_t *stack = stack_init();
	stack_push(stack, build_affine_matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1));

	return stack;
}

void push_affine_matrix(stack_t *stack)
{
	affine_matrix_t *src = stack_peek(stack);
	stack_push(stack, build_affine_matrix(
			src->v0.x, src->v0.y, src->v0.z, src->v0.p,
			src->v1.x, src->v1.y, src->v1.z, src->v1.p,
			src->v2.x, src->v2.y, src->v2.z, src->v2.p,
			src->v3.x, src->v3.y, src->v3.z, src->v3.p));
}

void identity(stack_t *stack)
{
	affine_matrix_t *a = stack_peek(stack);
	affine_matrix_t b = {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}};
	*a = b;
}

void translate(stack_t *stack, float x, float y, float z)
{
	affine_matrix_t *current = stack_peek(stack);
	affine_matrix_t new = {
			{1, 0, 0, x},
			{0, 1, 0, y},
			{0, 0, 1, z},
			{0, 0, 0, 1}};
	affine_matrix_multiply(current, &new);
}

void rotate(stack_t *stack, float angle, float x, float y, float z)
{
	float c = cos(angle);
	float s = sin(angle);

	affine_matrix_t *current = stack_peek(stack);
	affine_matrix_t new = {
			{x * x * (1 - c) + c, y * x * (1 - c) - z * s, z * x * (1 - c) + y * s, 0},
			{x * y * (1 - c) + z * s, y * y * (1 - c) + c, z * y * (1 - c) - x * s, 0},
			{x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c, 0},
			{0, 0, 0, 1}};
	affine_matrix_multiply(current, &new);
}

void scale(stack_t *stack, float x, float y, float z)
{
	affine_matrix_t *current = stack_peek(stack);
	affine_matrix_t new = {
			{x, 0, 0, 0},
			{0, y, 0, 0},
			{0, 0, z, 0},
			{0, 0, 0, 1}};
	affine_matrix_multiply(current, &new);
}

affine_vector_t apply_affine_matrix(stack_t *stack, affine_vector_t *point)
{
	if(point == NULL)
	{
		affine_vector_t temp = {0, 0, 0, 1};
		point = &temp;
	}

	affine_matrix_t *affine_matrix = stack_peek(stack);
	affine_vector_t result = {
			affine_matrix->v0.x * point->x + affine_matrix->v0.y * point->y + affine_matrix->v0.z * point->z + affine_matrix->v0.p * point->p,
			affine_matrix->v1.x * point->x + affine_matrix->v1.y * point->y + affine_matrix->v1.z * point->z + affine_matrix->v1.p * point->p,
			affine_matrix->v2.x * point->x + affine_matrix->v2.y * point->y + affine_matrix->v2.z * point->z + affine_matrix->v2.p * point->p,
			affine_matrix->v3.x * point->x + affine_matrix->v3.y * point->y + affine_matrix->v3.z * point->z + affine_matrix->v3.p * point->p};

	return result;
}

void place_object(sco_file_t *sco_file, char *name, stack_t *stack)
{
	mission_object_t *more_mission_objects = (mission_object_t *)realloc(
			sco_file->mission_objects,
			(sco_file->num_mission_objects + 1) * sizeof(mission_object_t));
	if(!more_mission_objects)
	{
		printf("ERROR: out of memory\n");
		return;
	}
	sco_file->mission_objects = more_mission_objects;

	affine_vector_t x = {1, 0, 0, 0};
	affine_vector_t y = {0, 1, 0, 0};
	affine_vector_t z = {0, 0, 1, 0};
	vector_wrapper_t v0, v1, v2, o;
	v0.affine_vector = apply_affine_matrix(stack, &x);
	v1.affine_vector = apply_affine_matrix(stack, &y);
	v2.affine_vector = apply_affine_matrix(stack, &z);
	o.affine_vector = apply_affine_matrix(stack, NULL);
	vector_t scale = {vector_length(&v0.vector), vector_length(&v1.vector), vector_length(&v2.vector)};
	normalize_vector(&v0.vector);
	normalize_vector(&v1.vector);
	normalize_vector(&v2.vector);

	mission_object_t *mission_object = &sco_file->mission_objects[sco_file->num_mission_objects];
	mission_object->id = name;
	mission_object->meta_type = 0;
	mission_object->sub_kind_no = 743;
	mission_object->variation_id = 0;
	mission_object->variation_id_2 = 0;
	mission_object->position.v0 = v0.vector;
	mission_object->position.v1 = v1.vector;
	mission_object->position.v2 = v2.vector;
	mission_object->position.o = o.vector;
	mission_object->scale = scale;
	print_mission_object(mission_object);
	printf("\n");

	++sco_file->num_mission_objects;
}

void pop_affine_matrix(stack_t *stack)
{
	free(stack_pop(stack));
}

void affine_free(stack_t *stack)
{
	while(!stack_is_empty(stack))
		pop_affine_matrix(stack);

	stack_free(stack);
}

