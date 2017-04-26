#include <stdio.h>
#include <stdlib.h>

#include "integrate.h"
#include "quadrant.h"


int main(int argc, char **argv) {
    printf("#### Calc PI\n");

    int steps;
    if (argc > 1)
    {
        steps = atoi(argv[1]);
    } else
    {
        steps = 1000000;
    }

    double pi = integrate(simpleCircle, 0, 1, steps) * 4;

    printf("Pi: %.16f\n",pi);

    return EXIT_SUCCESS;
}
