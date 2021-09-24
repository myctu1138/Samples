#ifndef _FFT_H
#define _FFT_H

#include <complex>
#include <vector>

using namespace std; 

#define PI (3.14159265358979323846)

void ParallelFFT2D(double *inputSignal, double *outputSignal, int w, int h);

void ParallelInverseFFT2D(double *inputSignal, double *outputSignal, int w, int h);

#endif