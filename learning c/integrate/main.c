#include <stdio.h>
#include <math.h>
#include "integrate.h"

double circle (double x){
    return sqrt(1 - pow(x,2));
}

double linear(double x){
    return x;
}

int main() {
    printf("Hello, World!\n");


    printf("%f\n",integrate(linear,1,2,100));

    return 0;
}