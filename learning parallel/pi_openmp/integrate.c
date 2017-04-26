//
// Created by Martin Weber on 12/04/2017.
//
#include <stdio.h>
#include <stdlib.h>

#include "integrate.h"

double integrate(double (*f)(double x), double l, double r, int steps){
	int i = 0;
    double AGesamt = 0.0;
    double deltaX;

	int max_thread = omp_get_max_threads();
	int threadStep = steps / max_thread;

	double A[max_thread];	

    if (l > r) {
        double temp = l;
        l = r;
        r = temp;
    }
	
	deltaX = (r-l)/steps;
	/*for ( i=0; i < max_thread; i++ ){
		A[i] = 0;
	}*/ 

	#pragma omp parallel
	{	
		int j = 0;
		int startStep = 0;
		int stopStep = 0;
		double AThread = 0;
		int current_thread = omp_get_thread_num();
		
		startStep = (threadStep * current_thread) + 1;
	
		if (omp_get_thread_num() + 1 == max_thread){
			stopStep = steps;
		}else{
			stopStep = threadStep * (current_thread + 1);
		}
		
		printf("Aktueller Thread: %d\t", current_thread);
		printf("Start: %d\t", startStep);
		printf("Stop: %d\n", stopStep);

		for(j = startStep; j <= stopStep; j++){
			AThread += f(l + (j * deltaX)) * deltaX; 
    	}
		#pragma omp critical 
		{  //Nur immer ein Thread Zugriff
			AGesamt += AThread;
		}
//		A[omp_get_thread_num()] = AThread;

	
	}

	/*for (i = 0; i < max_thread; i++ ){
		AGesamt += A[i];
	} */

    return AGesamt;
}
