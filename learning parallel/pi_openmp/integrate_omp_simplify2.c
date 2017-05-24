//
// Created by Martin Weber on 12/04/2017.
//
#include <stdio.h>
#include <stdlib.h>

#include "integrate.h"

double integrate_simplify2(double (*f)(double x), double l, double r, int steps){
    double AGesamt = 0.0;
    double deltaX;

    if (l > r) {
        double temp = l;
        l = r;
        r = temp;
    }

    deltaX = (r-l)/steps;
	
	#pragma omp parallel for
    for(int i = 1; i <= steps; i++){
		double x = l + (i * deltaX);
		
		#pragma omp critical 
		{  //Nur immer ein Thread Zugriff
	        AGesamt += f(x) * deltaX;
		}
    }

    return AGesamt;
}
