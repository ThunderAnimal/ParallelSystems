/**
 * Test for functions in core.c
 *
 * We don't need to test anything here really.
 */
#include <assert.h>

/* Include the module under testing */
#include "core.c"

/******************************************************/

static void test__sub_calculates_diff(void)
{
	assert(0 == sub(3,3));
	assert(0 == sub(-1,-1));
	assert(5 == sub(8,3));
	assert(-5 == sub(3,8));
}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
	test__sub_calculates_diff();
	return 0;
}
