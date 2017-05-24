//
// Created by Martin Weber on 12/04/2017.
//
#include <stdio.h>
#include <stdlib.h>

double integrate_simplify3(double (*f)(double x), double l, double r, int steps){
    double AGesamt = 0.0;
    double deltaX;
	int i;

    if (l > r) {
        double temp = l;
        l = r;
        r = temp;
    }

    deltaX = (r-l)/steps;
	
	#pragma omp parallel for reduction(+:AGesamt)
    for(i = 1; i <= steps; i++){
		double x = l + (i * deltaX);
        AGesamt += f(x) * deltaX;
    }

    return AGesamt;
}
