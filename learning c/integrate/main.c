#include <stdio.h>
#include <stdlib.h>

#include "integrate.h"
#include "functions.h"

int main() {
    printf("#### Calc PI\n");

    double pi = integrate(simpleCircle, 0, 1, 1000000000) * 4;

    printf("Pi: %.16f\n",pi);

    return EXIT_SUCCESS;
}