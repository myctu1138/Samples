#include "fft.h"

void BitReversing(vector<complex<double>> &inputSignal, vector<complex<double>> &outputSignal, int size, int step, int offset) 
{
  int bitsCount = 0;

  //ќпределение количества бит дл€ бит-реверсировани€. ѕолучаетс€, что bitsCount = log2(size).
  for( int tmp_size = size; tmp_size > 1; tmp_size/=2, bitsCount++ );

  //¬ыполнение бит-реверсировани€
  //ind - индекс элемента в  массиве input
  //revInd - соответствующие индексу ind индекс (бит-реверсивный) в массиве output
  for(int ind = 0; ind < size; ind++)
  {
    int mask = 1 << (bitsCount - 1);
    int revInd = 0;

    for(int i=0; i<bitsCount; i++)
    {
	    bool val = ind & mask;
	    revInd |= val << i;
	    mask = mask >> 1;
    }

    outputSignal[revInd*step + offset] = inputSignal[ind*step + offset];
  }
}

void Butterfly(vector<complex<double>> &signal, complex<double> u, int offset, int butterflySize, int step, int off) 
{
  complex<double> tem = signal[off + step*(offset +  butterflySize)] * u;

  signal[off + step*(offset +  butterflySize)] = signal[off + step*(offset)] - tem;
  signal[off + step*(offset)] += tem;
}

void SerialFFTCalculation(vector<complex<double>> &signal, int first, int size, int step, int offset, bool forward=true) 
{
  if(size==1)
    return;

  double const coeff=2.0*PI/size;

  SerialFFTCalculation( signal, first, size/2, step, offset, forward);
  SerialFFTCalculation( signal, first + size/2, size/2, step, offset, forward);

  for (int  j=first; j<first+size/2; j++ )
    if(forward)
      Butterfly(signal, complex<double>(cos(-j*coeff), sin(-j*coeff)), j , size/2, step, offset );
    else
      Butterfly(signal, complex<double>(cos(j*coeff), sin(j*coeff)), j , size/2, step, offset );
}

void SerialFFT1D(vector<complex<double>> &inputSignal, vector<complex<double>> &outputSignal, int size, int step, int offset)  
{
  BitReversing(inputSignal, outputSignal, size, step, offset);
  SerialFFTCalculation(outputSignal, 0, size, step, offset);
}

void SerialFFT2D(vector<complex<double>> inputSignal, vector<complex<double>> &outputSignal, int w, int h)  
{
  vector<complex<double>> tem(w*h);

  for(int i=0; i<w; i++)
    SerialFFT1D(inputSignal, tem, h, w, i);


  for(int j=0; j<h; j++)
    SerialFFT1D(tem, outputSignal, w, 1, h*j);
}

void SerialInverseFFT1D(vector<complex<double>> &inputSignal, vector<complex<double>> &outputSignal, int size, int step, int offset)  
{
  BitReversing(inputSignal, outputSignal, size, step, offset);
  SerialFFTCalculation(outputSignal, 0, size, step, offset, false);

  for (int  j=0; j<size; j++ )
    outputSignal[offset + step*j]/=size;
}

void SerialInverseFFT2D(vector<complex<double>> inputSignal, vector<complex<double>> &outputSignal, int w, int h)  
{
  vector<complex<double>> tem(w*h);

  for(int i=0; i<w; i++)
    SerialInverseFFT1D(inputSignal, tem, h, w, i);

  for(int j=0; j<h; j++)
    SerialInverseFFT1D(tem, outputSignal, w, 1, h*j);

}