#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "affine.h"
#include "stack.h"

int main()
{
	stack_t *stack = affine_init();
	affine_vector_t origin;
	origin.x = 5; origin.y = 5; origin.z = 0; origin.p = 1;

	translate(stack, origin.x, origin.y, origin.z);
	int lcv;
	for(lcv = 0; lcv < 8; ++lcv)
	{
		push_affine_matrix(stack);
			rotate(stack, 2 * M_PI * lcv / 8, 0, 0, 1);
			translate(stack, 1, 0, 0);
			scale(stack, 0.5, 0.5, 0.5);
			affine_vector_t point = apply_affine_matrix(stack, NULL);
			printf("{%f, %f, %f, %f}\n", point.x, point.y, point.z, point.p);
		pop_affine_matrix(stack);
	}

	affine_free(stack);

	return EXIT_SUCCESS;
}

