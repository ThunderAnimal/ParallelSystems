//
// Created by Martin Weber on 12/04/2017.
//
#include <stdio.h>
#include "integrate.h"

double integrate(double (*f)(double x), double l, double r, int steps){
    double A = 0.0;
    double deltaX = 0.0;
    int i;

    if (l > r) {
        double temp = l;
        l = r;
        r = temp;
    }

    deltaX = (r-l)/steps;
    for(i = 1; i <= steps; i++){
        A += f(l + (i * deltaX)) * deltaX;
    }
    return A;
}
