#include "fft.h"

#include "intrin.h"

unsigned char rev[256]={
0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240, 8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88,
216, 56, 184, 120, 248, 4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244, 12, 140, 76, 204, 44, 
172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252, 2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 
242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250, 6, 134, 70, 198, 38, 166, 102, 230, 22, 
150, 86, 214, 54, 182, 118, 246, 14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254, 1, 129, 65, 
193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 
185, 121, 249, 5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245, 13, 141, 77, 205, 45, 173, 109,
237, 29, 157, 93, 221, 61, 189, 125, 253, 3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243, 11, 
139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251, 7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 
215, 55, 183, 119, 247, 15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255};

union IntUni
{
  unsigned int integer;
  unsigned char byte[4];
};

void BitReversing(double *inputSignal, double *outputSignal, int size) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  IntUni i, j;
  j.integer=0;

  for(i.integer=0; i.integer<size/2; i.integer++)
  {
    j.byte[0]=rev[i.byte[3]];
    j.byte[1]=rev[i.byte[2]];
    j.byte[2]=rev[i.byte[1]];
    j.byte[3]=rev[i.byte[0]];

    j.integer = j.integer >> (32-m);

    outputSignal[2*i.integer] = inputSignal[2*j.integer];
    outputSignal[2*i.integer+1] = inputSignal[2*j.integer+1];
    outputSignal[2*i.integer+size] = inputSignal[2*j.integer+2];
    outputSignal[2*i.integer+1+size] = inputSignal[2*j.integer+1+2];
  }
}

void SerialFFTCalculation(double *mas, int size) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

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
        double temR = mas[2 * (j + offset +  butterflySize)] * uR - mas[2 * (j + offset +  butterflySize) + 1] * uI,
               temI = mas[2 * (j + offset +  butterflySize)] * uI + mas[2 * (j + offset +  butterflySize) + 1] * uR;

	      mas[2 * (j + offset +  butterflySize)] = mas[2 * (j + offset)] - temR;
        mas[2 * (j + offset +  butterflySize) + 1] = mas[2 * (j + offset) + 1] - temI;
		    
        mas[2 * (j + offset)] += temR;
        mas[2 * (j + offset) + 1] += temI;

        uRtmp=uR;
        uR=uR*wR-uI*wI;
        uI=uI*wR+uRtmp*wI;
      }
    }
  }
}

void SerialFFT1D(double *inputSignal, double *outputSignal, int size)  
{
  BitReversing(inputSignal, outputSignal, size);
  SerialFFTCalculation(outputSignal, size);
}

void ParallelFFT2D(double *inputSignal, double *outputSignal, int w, int h)  
{
  #pragma omp parallel
  {
    double *inW, *outW, *inH, *outH;

    inW = new double[2*w];
    outW = new double[2*w];
    inH = new double[2*h];
    outH = new double[2*h];


    #pragma omp for
    for(int i=0; i<w; i++)
    {
      for(int j=0; j<h; j++)
      {
        inH[2*j] = inputSignal[2*(j*w+i)];
        inH[2*j+1] = inputSignal[2*(j*w+i)+1];
      }

      SerialFFT1D(inH, outH, h);

      for(int j=0; j<h; j++)
      {
        outputSignal[2*(j*w+i)] = outH[2*j];
        outputSignal[2*(j*w+i)+1] = outH[2*j+1];
      }
    }

    #pragma omp for
    for(int j=0; j<h; j++)
    {
      for(int i=0; i<w; i++)
      {
        inW[2*i] = outputSignal[2*(j*w+i)];
        inW[2*i+1] = outputSignal[2*(j*w+i)+1];
      }

      SerialFFT1D(inW, outW, w);

      for(int i=0; i<w; i++)
      {
        outputSignal[2*(j*w+i)] = outW[2*i];
        outputSignal[2*(j*w+i)+1] = outW[2*i+1];
      }
    }

    delete[] inW;
    delete[] inH;
    delete[] outW;
    delete[] outH;
  }
}

void SerialInverseFFTCalculation(double *mas, int size) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

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
        double temR = mas[2 * (j + offset +  butterflySize)] * uR - mas[2 * (j + offset +  butterflySize) + 1] * uI,
               temI = mas[2 * (j + offset +  butterflySize)] * uI + mas[2 * (j + offset +  butterflySize) + 1] * uR;

	      mas[2 * (j + offset +  butterflySize)] = mas[2 * (j + offset)] - temR;
        mas[2 * (j + offset +  butterflySize) + 1] = mas[2 * (j + offset) + 1] - temI;
		    
        mas[2 * (j + offset)] += temR;
        mas[2 * (j + offset) + 1] += temI;
        
        uRtmp=uR;
        uR=uR*wR-uI*wI;
        uI=uI*wR+uRtmp*wI;
      }
    }
  }
}

void SerialInverseFFT1D(double *inputSignal, double *outputSignal, int size)  
{
  BitReversing(inputSignal, outputSignal, size);
  SerialInverseFFTCalculation(outputSignal, size);

  double _size(size);

  for (int  j=0; j<2*size; j++ )
    outputSignal[j]/=_size;
}

void ParallelInverseFFT2D(double *inputSignal, double *outputSignal, int w, int h)  
{
  #pragma omp parallel
  {
    double *inW, *outW, *inH, *outH;

    inW = new double[2*w];
    outW = new double[2*w];
    inH = new double[2*h];
    outH = new double[2*h];

    #pragma omp for
    for(int i=0; i<w; i++)
    {
      for(int j=0; j<h; j++)
      {
        inH[2*j] = inputSignal[2*(j*w+i)];
        inH[2*j+1] = inputSignal[2*(j*w+i)+1];
      }

      SerialInverseFFT1D(inH, outH, h);

      for(int j=0; j<h; j++)
      {
        outputSignal[2*(j*w+i)] = outH[2*j];
        outputSignal[2*(j*w+i)+1] = outH[2*j+1];
      }
    }

    #pragma omp for
    for(int j=0; j<h; j++)
    {
      for(int i=0; i<w; i++)
      {
        inW[2*i] = outputSignal[2*(j*w+i)];
        inW[2*i+1] = outputSignal[2*(j*w+i)+1];
      }

      SerialInverseFFT1D(inW, outW, w);

      for(int i=0; i<w; i++)
      {
        outputSignal[2*(j*w+i)] = outW[2*i];
        outputSignal[2*(j*w+i)+1] = outW[2*i+1];
      }
    }

    delete[] inW;
    delete[] inH;
    delete[] outW;
    delete[] outH;
  }
}