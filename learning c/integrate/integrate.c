//
// Created by Martin Weber on 12/04/2017.
//
#include "integrate.h"

double integrate(double (*f)(double x), double l, double r, int steps){
    double A = 0.0;
    double deltaX;
    int i;

    if (l > r) {
        double temp = l;
        l = r;
        r = temp;
    }

    deltaX = (r-l)/steps;

    for(i = 1; i <= steps; i++){
		double x = l + (i * deltaX) 
        A += f(x) * deltaX;
    }

    return A;
}
