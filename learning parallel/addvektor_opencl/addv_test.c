//
// Created by Martin Weber on 13/04/2017.
//

#include <assert.h>
#include <math.h>

#include "addvektorcl.c"

static void compare_array(float *a, float *b, int len){
    for(int i = 0; i < len; i++){
        assert(a[i] == b[i]);
    }
}

static void test__addv1(void)
{ 
    int len = 3;
    float a[3] = {0.0, 1.5, 2};
    float b[3] = {1.0, 0.5, 3};

    float c[3];
    for(int i = 0; i < len; i++){
        c[i] = 0;
    }

    float result[3] = {1.0, 2.0, 5};

    vadd(a, b, c, len);
    compare_array(result, c, len);
}

static void test__addv2(void)
{ 
    int len = 5;
    float a[5] = {0.0, 1.0, 2.0, 3.0, 4.0};
    float b[5] = {1.0, 0.5, 3, 4 ,5};

    float c[5];
    for(int i = 0; i < len; i++){
        c[i] = 0;
    }

    float result[5] = {1.0, 1.5, 5, 7, 9};

    vadd(a, b, c, len);
    compare_array(result, c, len);
}

static void test__addv3(void)
{ 
    int len = 3;
    float a[3] = {-1.0, -1.5, 2};
    float b[3] = {1.0, 0.5, -3};

    float c[3];
    for(int i = 0; i < len; i++){
        c[i] = 0;
    }

    float result[3] = {0, -1, -1};

    vadd(a, b, c, len);
    compare_array(result, c, len);
}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
    test__addv1();
    test__addv2();
    test__addv3();
    return 0;
}

