/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */
#include <stdio.h>
#include <omp.h>

#include "core.h"

/**
 * Prints out hello world.
 */
void hello_world(void)
{
	#pragma omp parallel
	{
		printf("Hello World (tid=%d)\n", omp_get_thread_num());
	}
}


/**
 * This is a simple function to add two integer
 * values. It main purpose is to demonstrate
 * the testing.
 */
int add(int x, int y)
{
	return x+y;
}
