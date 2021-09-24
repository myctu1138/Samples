#include "fft.h"

void BitReversing(complex<double> *inputSignal, complex<double> *outputSignal, int size, int step, int offset) 
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

void SerialFFTCalculation(complex<double> *mas, int size, int step, int off) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  complex<double> *_mas = mas + off;

  for (int p=0; p<m; p++ )
  {
    int butterflyOffset = 1 << (p+1) ;  
    int butterflySize = butterflyOffset >> 1 ;  
    
    double alpha=-2.0*PI/butterflyOffset;
    double wR=cos( alpha  ), wI=sin( alpha );

    for (int i=0; i<size/butterflyOffset; i++ )
    {
      double uR=1.0, uI=0.0;
      double uRtmp;

      int offset = i * butterflyOffset;
      
      for (int  j=0; j<butterflySize; j++ )
      {
        complex<double> tem = _mas[(j + offset +  butterflySize)*step] * complex<double>(uR, uI);

		    _mas[(j + offset + butterflySize)*step] = _mas[(j + offset)*step] - tem;
		    _mas[(j + offset)*step] += tem;

        uRtmp=uR;
        uR=uR*wR-uI*wI;
        uI=uI*wR+uRtmp*wI;
      }
    }
  }
}

void SerialFFT1D(complex<double> *inputSignal, complex<double> *outputSignal, int size, int step, int offset)  
{
  BitReversing(inputSignal, outputSignal, size, step, offset);
  SerialFFTCalculation(outputSignal, size, step, offset);
}

void SerialFFT2D(complex<double> *inputSignal, complex<double> *outputSignal, int w, int h)  
{
  complex<double> *tem = new complex<double>[w*h];

  for(int i=0; i<w; i++)
    SerialFFT1D(inputSignal, tem, h, w, i);


  for(int j=0; j<h; j++)
    SerialFFT1D(tem, outputSignal, w, 1, h*j);

  delete[] tem;
}

void SerialInverseFFTCalculation(complex<double> *mas, int size, int step, int off) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  complex<double> *_mas = mas + off;

  for (int p=0; p<m; p++ )
  {
    int butterflyOffset = 1 << (p+1) ;  
    int butterflySize = butterflyOffset >> 1 ;  
    
    double alpha=2.0*PI/butterflyOffset;
    double wR=cos( alpha  ), wI=sin( alpha );

    for (int i=0; i<size/butterflyOffset; i++ )
    {
      double uR=1.0, uI=0.0;
      double uRtmp;

      int offset = i * butterflyOffset;
      
      for (int  j=0; j<butterflySize; j++ )
      {
        complex<double> tem = _mas[(j + offset +  butterflySize)*step] * complex<double>(uR, uI);

		    _mas[(j + offset + butterflySize)*step] = _mas[(j + offset)*step] - tem;
		    _mas[(j + offset)*step] += tem;

        uRtmp=uR;
        uR=uR*wR-uI*wI;
        uI=uI*wR+uRtmp*wI;
      }
    }
  }
}

void SerialInverseFFT1D(complex<double> *inputSignal, complex<double> *outputSignal, int size, int step, int offset)  
{
  BitReversing(inputSignal, outputSignal, size, step, offset);
  SerialInverseFFTCalculation(outputSignal, size, step, offset);

  complex<double> _size(size);

  for (int  j=0; j<size; j++ )
    outputSignal[offset + step*j] /= _size;
}

void SerialInverseFFT2D(complex<double> *inputSignal, complex<double> *outputSignal, int w, int h)  
{
  complex<double> *tem = new complex<double>[w*h];

  for(int i=0; i<w; i++)
    SerialInverseFFT1D(inputSignal, tem, h, w, i);

  for(int j=0; j<h; j++)
    SerialInverseFFT1D(tem, outputSignal, w, 1, h*j);

  delete[] tem;
}