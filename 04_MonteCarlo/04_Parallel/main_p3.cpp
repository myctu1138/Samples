// ������� ������ ����������� ������������ ���� ������� ������������ ����
// ��������������� ����-������ �� �����, ���������� ���������� ����� 
//   � ����������� �������

// ������ ������� (���� �������) ������������ �� ������� �����-������
// ������������� ������������ ���������� ���� ������� �����-�����
// ������������ ������������� ������-��������� � �����-��������� �����

// ***** ���������� ������������ ������ *****

#include  <assert.h>
#include  <stdio.h>
#include  <math.h>
#include  <time.h>
#include  <mkl.h>
#include "omp.h"

// ����� ������������� ���������� �����, �� ������� ����������
// ����������� � �������� ������. � ���������� ������ - 1 ��������� � 1 �����

// ������ �����: (�������������� ����������� ��������)
// B(t) - ���� ���������
// S(t) - ���� �����
// W - ����������� ��������� �������:
//    W(0) = 0
//    W(t) - ���������� � ������������ 1
//    W(t) - W(s) - ����������� N(0, t-s), ��� 0 <= s < t

// dB(t) = R B(T) dt              - 
// dS(t) = S(t) (r dt - SIG dW), S(0) > 0 (����� S(0) = S0) 

//                      ��������� ������                                       
const double sig = 0.2;      // ������������� (% ������� 0.2 -> 20%)
const double r   = 0.05;     // ���������� ������ (% ������� 0.05 -> -5%)
const double T   = 3.0;      // ���� ���������� ������� (� �����)
const double s0  = 100.0;    // ���� ����� � ������ ������� t=0 (�.�.)
const double K   = 100.0;    // ������ - ���� �������, ������. � ������� (�.�.)

// ������ � ����� �����
clock_t start, finish;
// ����� ����������
double t;

// ���������� ���� ������� �� ������� �����-������
// C = S0*F(d1) - K * exp(-r * T) * F(d2)
// d1 = (ln(s0/K) + (r + sig*sig/2)*T) / (sig* sqrt(T))
// d2 = d1 - sig * sqrt(T)
// ��� F - ������� ������������ ����������� �������������
// ����� ��������� ���������� ������ �����
__declspec(noinline) double GetOptionPrice()
{
  double C;
  double p1[2], p2[2];

  start = clock();
  p1[0] = (log(s0 / K) + (r + sig * sig * 0.5) * T) / (sig * sqrt(T));
  p1[1] = p1[0] - sig * sqrt(T);
  vdCdfNorm(2, p1, p2);
  C  = s0 * p2[0] - K * exp((-1.0) * r * T) * p2[1];
  finish = clock();
  t = (double)(finish - start) / CLOCKS_PER_SEC;

  return C;
}

// ���������� ���� ������� ������� �����-�����
//
// C = exp(-r * T) * INTEGRAL{ max(S(T) - K, 0) fi(z) dz},
//   ��� S(T) - ���� ����� � ������ ��������� ����� �������� ������� (t = T),
//   fi(z) - ��������� ������������ ����������� �������������,
//   �������� ������� �� ���� �������� ���
//
// �������� ����������� ������� �����-�����:
//   ���������� ���� ����� � ������ ������� T (��������� ����� �������� �������)
//   S(T) = S(0) * exp ((r - sig * sig / 2.0) * T + sig * sqrt(T) * z),
//   z -> N(0, 1)
//   C = exp(-r * T) * SUMi{max(Si(T) - K, 0)} / nsamples
__declspec(noinline) double GetMCPrice(unsigned int nsamples, int method, unsigned int nthreads)
{
  double sum = 0.0;
  double s;
  double tmp1 = (r - sig * sig * 0.5) * T;
  double tmp2 = sig * sqrt(T);

  int count;

   // ������ ������ ��� ��������� �����
  const unsigned int bufsize = 1000;
  const unsigned int seed[2] = {2000000000, 2000000000};
  assert(nsamples % bufsize == 0);

  start = clock();
#pragma omp parallel private(s) num_threads(nthreads)
  {
    count = omp_get_num_threads();
    int num = omp_get_thread_num();

    // ����� ��� ��������� �����
    double *gauss = new double[bufsize];
    // ����� ��� MKL ����������
    VSLStreamStatePtr stream;
    // �������������� ������� ��������� MCG59 ��� SOBOL
    if (method == 0)
      vslNewStreamEx(&stream, VSL_BRNG_MCG59, 2, seed);
    if (method == 1)
      vslNewStream(&stream, VSL_BRNG_SOBOL, 1);
  
    // �������, ��� nsamples ������� ������ �� (count * bufsize)
    assert(nsamples % (count * bufsize) == 0);
    // ������� "skip-ahead" ��� ����������� ������������� ������������������ ��������� �����
    vslSkipAheadStream(stream, nsamples / count * num);

#pragma omp for reduction(+:sum)
    for (unsigned int portion = 0; portion < nsamples / bufsize; portion++)
    {
      vdRngGaussian(VSL_METHOD_DGAUSSIAN_ICDF, stream, bufsize, gauss, 0.0, 1.0);

      for (int i = 0; i < bufsize; i++)
      {
        double payoff;
        s = s0 * exp(tmp1 + tmp2 * gauss[i]);
        payoff = s - K;
        if (payoff > 0.0)
          sum += payoff;
      }
    }
    vslDeleteStream(&stream);
    delete [] gauss;
  }
  sum = sum / nsamples * exp(-r * T);

  finish = clock();
  t = (double)(finish - start) / CLOCKS_PER_SEC;

  return sum;
}

int main(int argc, char *argv[])
{
  if (argc == 3)
  {
    unsigned int nsamples = atoi(argv[1]);
    unsigned int nthreads = atoi(argv[2]);

  // ����� ����� ������ ������� ��� ������ �������� �������
    printf("%d;%d;%.15f;", nthreads, nsamples, GetOptionPrice());
    printf("%.15f;%f;", GetMCPrice(nsamples, 0, nthreads), t);
    printf("%.15f;%f\n", GetMCPrice(nsamples, 1, nthreads), t);

    return 0;
  }
  else
    return 1;
}
