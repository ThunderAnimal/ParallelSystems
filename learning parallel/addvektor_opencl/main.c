#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "addvektorcl.h"

main(){
	float a[3] = {0.0, 1.5, 2};
	float b[3] = {1.0, 0.5, 3};

	float c[3];
	for(int i = 0; i < 3; i++){
		c[i] = 0;
	}
	
	printf("Vektoraddition:\n");
	
	vadd(a, b, c, 3);

	printf("Vekrot a ");
	for(int i = 0; i < 3; i++){
		printf("%f\t", a[i]);
	}
	printf("\n");
	printf("Vektor b ");
	for(int i = 0; i < 3; i++){
		printf("%f\t", b[i]);
	}
	printf("\n");

	printf("Ergebnis\n");
	for(int i = 0; i < 3; i++){
		printf("%f\t", c[i]);
	}
	printf("\n");
}