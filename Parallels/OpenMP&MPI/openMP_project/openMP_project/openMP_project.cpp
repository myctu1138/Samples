// openMP_project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "2nd.h"

int main()
{
	function();
	/*
	int nThreads, tid;
	#pragma omp parallel private(nThreads, tid)
	{
		tid = omp_get_thread_num();
		printf("Hello World from thread = %d\n", tid);

		if (tid == 0) {
			nThreads = omp_get_thread_num();
			printf("Number of threads = %d\n", nThreads);
		}
	}
	*/
}