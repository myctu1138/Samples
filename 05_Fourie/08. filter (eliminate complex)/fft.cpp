#include "fft.h"

void BitReversing(double *inputSignal, double *outputSignal, int size, int step, int offset) 
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

    outputSignal[2*revInd*step + 2*offset] = inputSignal[2*ind*step + 2*offset];
    outputSignal[2*revInd*step + 2*offset + 1] = inputSignal[2*ind*step + 2*offset + 1];
  }
}

void SerialFFTCalculation(double *mas, int size, int step, int off) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  double *_mas = mas + 2*off;

  for (int p=0; p<m; p++ )
  {
    int butterflyOffset = 1 << (p+1) ;  
    int butterflySize = butterflyOffset >> 1 ;  
    
    double alpha=-2.0*PI/butterflyOffset;
    double wR=cos( alpha  ), wI=sin( alpha );

    double uRtmp;
    double temI;

    for (int i=0; i<size/butterflyOffset; i++ )
    {
      double uR=1.0, uI=0.0;

      int offset = i * butterflyOffset;
      
      for (int  j=0; j<butterflySize; j++ )
      {
        double temR = _mas[2 * (j + offset +  butterflySize)*step] * uR - _mas[2 * (j + offset +  butterflySize)*step + 1] * uI;
               temI = _mas[2 * (j + offset +  butterflySize)*step] * uI + _mas[2 * (j + offset +  butterflySize)*step + 1] * uR;

        _mas[2 * (j + offset +  butterflySize)*step] = _mas[2 * (j + offset)*step] - temR;
        _mas[2 * (j + offset +  butterflySize)*step + 1] = _mas[2 * (j + offset)*step + 1] - temI;
		    
        _mas[2 * (j + offset)*step] += temR;
        _mas[2 * (j + offset)*step + 1] += temI;

        uRtmp=uR;
        uR=uR*wR-uI*wI;
        uI=uI*wR+uRtmp*wI;
      }
    }
  }
}

void SerialFFT1D(double *inputSignal, double *outputSignal, int size, int step, int offset)  
{
  BitReversing(inputSignal, outputSignal, size, step, offset);
  SerialFFTCalculation(outputSignal, size, step, offset);
}

void SerialFFT2D(double *inputSignal, double *outputSignal, int w, int h)  
{
  double *tem = new double[2*w*h];

  for(int i=0; i<w; i++)
    SerialFFT1D(inputSignal, tem, h, w, i);


  for(int j=0; j<h; j++)
    SerialFFT1D(tem, outputSignal, w, 1, h*j);

  delete[] tem;
}

void SerialInverseFFTCalculation(double *mas, int size, int step, int off) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  double *_mas = mas + 2*off;

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
        double temR = _mas[2 * (j + offset +  butterflySize)*step] * uR - _mas[2 * (j + offset +  butterflySize)*step + 1] * uI,
               temI = _mas[2 * (j + offset +  butterflySize)*step] * uI + _mas[2 * (j + offset +  butterflySize)*step + 1] * uR;

	      _mas[2 * (j + offset +  butterflySize)*step] = _mas[2 * (j + offset)*step] - temR;
        _mas[2 * (j + offset +  butterflySize)*step + 1] = _mas[2 * (j + offset)*step + 1] - temI;
		    
        _mas[2 * (j + offset)*step] += temR;
        _mas[2 * (j + offset)*step + 1] += temI;
        
        uRtmp=uR;
        uR=uR*wR-uI*wI;
        uI=uI*wR+uRtmp*wI;
      }
    }
  }
}

void SerialInverseFFT1D(double *inputSignal, double *outputSignal, int size, int step, int offset)  
{
  BitReversing(inputSignal, outputSignal, size, step, offset);
  SerialInverseFFTCalculation(outputSignal, size, step, offset);

  double _size(size);

  for (int  j=0; j<size; j++ )
  {
    outputSignal[2*(offset+j*step)]/=_size;
    outputSignal[2*(offset+j*step)+1]/=_size;
  }
}

void SerialInverseFFT2D(double *inputSignal, double *outputSignal, int w, int h)  
{
  double *tem=new double[2*w*h];

  for(int i=0; i<w; i++)
    SerialInverseFFT1D(inputSignal, tem, h, w, i);

  for(int j=0; j<h; j++)
    SerialInverseFFT1D(tem, outputSignal, w, 1, h*j);

  delete[] tem;
}