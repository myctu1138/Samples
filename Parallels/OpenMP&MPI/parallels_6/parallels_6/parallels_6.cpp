// parallels_6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>
bool isPrime(int n);
int main(void)
{
	int NUMBER = 100, sum = 0;
#pragma omp parallel for reduction(+ : sum)
	for (int i = 0; i < NUMBER; i++)
	{
		if (isPrime(i)) {
			sum += i;
			printf("Prime %d executed by thread %d,"
				"temporal sum value is %d\n", i, omp_get_thread_num(), sum);
		}
	}
	printf("Sum is: %d\n", sum);
	system("pause");
}
bool isPrime(int n)
{
	// Corner cases 
	if (n <= 1)
		return false;
	if (n <= 3)
		return true;

	// This is checked so that we can skip 
	// middle five numbers in below loop 
	if (n % 2 == 0 || n % 3 == 0)
		return false;

	for (int i = 5; i * i <= n; i = i + 6)
		if (n % i == 0 || n % (i + 2) == 0)
			return false;

	return true;
}