/**
 * Test for functions in core.c
 *
 * We don't need to test anything here really.
 */
#include <assert.h>

/* Include the module under testing */
#include "core.c"

/******************************************************/

void test__calc_is_correct(void)
{
	calc();
}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
	test__calc_is_correct();
	return 0;
}
