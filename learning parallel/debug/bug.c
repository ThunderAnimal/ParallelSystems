/**
 * @file add.c
 *
 * This is the main driver of the program, i.e.,
 * the program, which is then used by the user.
 */
#include <stdio.h>

#include "core.h"

int main(int argc, char **argv)
{
	printf("result = %lf\n",calc());
	return 0;
}
