//
// Created by Martin Weber on 13/04/2017.
//

#include <assert.h>
#include <math.h>

#include "integrate.c"

static double yEquals1(double x){
    return 1;
}

static double simpleLinear(double x){
    return x;
}

static double simpleParabel(double x){
    return pow(x,2);
}

static void test__integrate(void)
{
    double deltaA = 0.0001;
	
	assert(fabs(integrate(yEquals1,3,5,1000000) - 2) < deltaA);
    assert(fabs(integrate(simpleLinear,1,2,100000) - 1.5) < deltaA);
    assert(fabs(integrate(simpleParabel,0,3,1000000) - 9) < deltaA);
}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
    test__integrate();
    return 0;
}

