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

void BitReversing(double *inputSignal, double *outputSignal, int size, int offset) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  IntUni i, j;
  j.integer=0;

  double *_inputSignal = inputSignal + 2*offset;
  double *_outputSignal = outputSignal + 2*offset;

  for(i.integer=0; i.integer<size/2; i.integer++)
  {
    j.byte[0]=rev[i.byte[3]];
    j.byte[1]=rev[i.byte[2]];
    j.byte[2]=rev[i.byte[1]];
    j.byte[3]=rev[i.byte[0]];

    j.integer = j.integer >> (32-m);

    _outputSignal[2*i.integer] = _inputSignal[2*j.integer];
    _outputSignal[2*i.integer+1] = _inputSignal[2*j.integer+1];
    _outputSignal[(2*i.integer+size)] = _inputSignal[(2*j.integer+2)];
    _outputSignal[(2*i.integer+size)+1] = _inputSignal[(2*j.integer+2)+1];
  }
}

void BitReversing(double *inputSignal, double *outputSignal, int size, int step, int offset) 
{
  int m=0;
  for(int tmp_size = size; tmp_size>1; tmp_size/=2, m++ );//size = 2^m

  IntUni i, j;
  j.integer=0;

  double *_inputSignal = inputSignal + 2*offset;
  double *_outputSignal = outputSignal + 2*offset;

  for(i.integer=0; i.integer<size/2; i.integer++)
  {
    j.byte[0]=rev[i.byte[3]];
    j.byte[1]=rev[i.byte[2]];
    j.byte[2]=rev[i.byte[1]];
    j.byte[3]=rev[i.byte[0]];

    j.integer = j.integer >> (32-m);

    _outputSignal[2*i.integer*step] = _inputSignal[2*j.integer*step];
    _outputSignal[2*i.integer*step+1] = _inputSignal[2*j.integer*step+1];
    _outputSignal[(2*i.integer+size)*step] = _inputSignal[(2*j.integer+2)*step];
    _outputSignal[(2*i.integer+size)*step+1] = _inputSignal[(2*j.integer+2)*step+1];
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