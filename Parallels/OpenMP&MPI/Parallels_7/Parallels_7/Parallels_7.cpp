// Parallels_7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

int main(int argc, char** argv)	{
	int niter = 1000000;
	double x, y;
	int i;
	int count = 0;
	double pi, z;
	int numthreads = 16;
#pragma omp parallel firstprivate(x,y,z,i) reduction(+:count) num_threads(numthreads)
	{
		srand((int)time(NULL) ^ omp_get_thread_num());

		for (i = 0; i < niter; i++)	{
			x = (rand() % 10000) / 10000.0;
			y = (rand() % 10000) / 10000.0;
			z = sqrt((x*x) + (y*y));
			if (z <= 1.0)
				++count;
		}
	}

	pi = 4.0 *((double)count/(double)(niter*numthreads));
	printf("Pi:%f\n", pi);

	return 0;
}