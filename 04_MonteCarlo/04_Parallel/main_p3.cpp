// Решение задачи определения справедливой цены опциона европейского типа
// Рассматривается колл-опцион на акцию, одномерный финансовый рынок 
//   в непрерывном времени

// Точное решение (цена опциона) определяется по формуле Блэка-Шоулза
// Дополнительно производится вычисление цены методом Монте-Карло
// Сравнивается использование псевдо-случайных и квази-случайных чисел

// ***** Корректная параллельная версия *****

#include  <assert.h>
#include  <stdio.h>
#include  <math.h>
#include  <time.h>
#include  <mkl.h>
#include "omp.h"

// Будем рассматривать финансовый рынок, на котором обращаются
// безрисковые и рисковые активы. В одномерном случае - 1 облигация и 1 акция

// Модель рынка: (геометрическое броуновское движение)
// B(t) - цена облигации
// S(t) - цена акции
// W - винеровский случайный процесс:
//    W(0) = 0
//    W(t) - непрерывна с вероятностью 1
//    W(t) - W(s) - принадлежит N(0, t-s), где 0 <= s < t

// dB(t) = R B(T) dt              - 
// dS(t) = S(t) (r dt - SIG dW), S(0) > 0 (далее S(0) = S0) 

//                      Параметры модели                                       
const double sig = 0.2;      // Волатильность (% годовых 0.2 -> 20%)
const double r   = 0.05;     // Процентная ставка (% годовых 0.05 -> -5%)
const double T   = 3.0;      // Срок исполнения опциона (в годах)
const double s0  = 100.0;    // Цена акции в момент времени t=0 (у.е.)
const double K   = 100.0;    // Страйк - цена покупки, зафикс. в опционе (у.е.)

// Начало и конец счета
clock_t start, finish;
// Время вычислений
double t;

// Вычисление цены опциона по формуле Блэка-Шоулза
// C = S0*F(d1) - K * exp(-r * T) * F(d2)
// d1 = (ln(s0/K) + (r + sig*sig/2)*T) / (sig* sqrt(T))
// d2 = d1 - sig * sqrt(T)
// Где F - функция стандартного нормального распределения
// Смысл остальных параметров описан ранее
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

// Вычисление цены опциона методом Монте-Карло
//
// C = exp(-r * T) * INTEGRAL{ max(S(T) - K, 0) fi(z) dz},
//   где S(T) - цена акции в момент окончания срока действия опциона (t = T),
//   fi(z) - плотность стандратного нормального распределения,
//   интеграл берется по всей числовой оси
//
// Интеграл вычисляется методом Монте-Карло:
//   Вычисление цены акции в момент времени T (истечение срока действия опциона)
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

   // Размер буфера для генерации чисел
  const unsigned int bufsize = 1000;
  const unsigned int seed[2] = {2000000000, 2000000000};
  assert(nsamples % bufsize == 0);

  start = clock();
#pragma omp parallel private(s) num_threads(nthreads)
  {
    count = omp_get_num_threads();
    int num = omp_get_thread_num();

    // Буфер для случайных чисел
    double *gauss = new double[bufsize];
    // Поток для MKL генератора
    VSLStreamStatePtr stream;
    // Инициализируем базовый генератор MCG59 или SOBOL
    if (method == 0)
      vslNewStreamEx(&stream, VSL_BRNG_MCG59, 2, seed);
    if (method == 1)
      vslNewStream(&stream, VSL_BRNG_SOBOL, 1);
  
    // Считаем, что nsamples делится нацело на (count * bufsize)
    assert(nsamples % (count * bufsize) == 0);
    // техника "skip-ahead" для корректного использования последовательности случайных чисел
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

  // Здесь будут вызовы функций для разных способов расчета
    printf("%d;%d;%.15f;", nthreads, nsamples, GetOptionPrice());
    printf("%.15f;%f;", GetMCPrice(nsamples, 0, nthreads), t);
    printf("%.15f;%f\n", GetMCPrice(nsamples, 1, nthreads), t);

    return 0;
  }
  else
    return 1;
}

