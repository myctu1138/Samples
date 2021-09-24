#include "2nd.h"

void function() {
	int i, t, n = 10;
	printf("Num of processors: %d\n", omp_get_num_procs());
	printf("Num max threads: %d\n", omp_get_max_threads());
	printf("Insert number of threads\n");
	scanf_s("%d", &t);
	if (t > omp_get_max_threads() * 2) {
		printf("Error invalid number of threads");
	}
	else {
		omp_set_num_threads(t);
		printf("Num Num Threads: %d\n", omp_get_num_threads());
		printf("Time 1: %f\n", omp_get_wtime());
#pragma omp parallel for default(none) schedule(runtime) \
private(i) shared(n)
		for (int i = 0; i < n; i++)
		{
			printf("Iteration %d executed by thread %d\n", i, omp_get_thread_num());
		}
	}
	printf("Time 2: %f\n", omp_get_wtime());
	getchar();
}