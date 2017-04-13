//
// Created by Martin Weber on 13/04/2017.
//

#include <assert.h>
#include <math.h>

#include "functions.c"
#include "integrate.c"

static void test__integrate(void)
{
    double deltaA = 0.00000001;

    assert(fabs(integrate(simpleLinear,1,2,1000) - 1.5) < deltaA);
    assert(fabs(integrate(simpleParabel,0,3,1000) - 9) < deltaA);

}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
    test__integrate();
    return 0;
}

