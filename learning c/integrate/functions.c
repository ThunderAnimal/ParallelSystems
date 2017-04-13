//
// Created by Martin Weber on 13/04/2017.
//
#include <math.h>
#include "functions.h"

double simpleCircle (double x){
    return sqrt(1 - pow(x,2));
}

double simpleLinear(double x){
    return x;
}
double simpleParabel(double x){
    return pow(x,2);
}