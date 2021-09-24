#ifndef _FFT_H
#define _FFT_H

#include <complex>
#include <vector>

using namespace std; 

#define PI (3.14159265358979323846)

void SerialFFT2D(complex<double> *inputSignal, complex<double> *outputSignal, int w, int h);

void SerialInverseFFT2D(complex<double> *inputSignal, complex<double> *outputSignal, int w, int h);

#endif