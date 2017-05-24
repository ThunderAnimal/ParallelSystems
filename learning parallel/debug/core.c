/**
 * Implements the core of the algorithm.
 *
 * @file core.c
 */

#include <stdio.h>
#include <omp.h>

#include "core.h"

double calc(void)
{
	int i;
	double sum = 0.0;
	double step;
	int num_steps = 1000000;

	step = 1.0/(double) num_steps;

	#pragma omp parallel for reduction(+:sum)
	for (i=0; i < num_steps; i++)
	{
		double x = (i+0.5)*step;
		sum += 1.0/(1.0+x*x);
	}
	sum *= step;
	return sum;
}
