#include "estructuras.cpp"
#include <complex>
#include<cmath>

using namespace::audio;

#ifndef FOURIER_CPP
#define FOURIER_CPP

namespace Fourier{


/**********************************************
* Funciones independientes.                   *
*                                             *
***********************************************/

int log2(int N) /*function to calculate the log2(.) of int numbers*/
{
  int k = N, i = 0;
  while (k)
  {
    k >>= 1;
    i++;
  }
  return i - 1;
}

int reverse(int N, int n) //calculating revers number
{
  int j, p = 0;
  for (j = 1; j <= log2(N); j++)
  {
    if (n & (1 << (log2(N) - j)))
      p |= 1 << (j - 1);
  }
  return p;
}

void ordina(complex<double> *f1, int N) //using the reverse order in the array
{
  complex<double> f2[N];
  for (int i = 0; i < N; i++)
    f2[i] = f1[reverse(N, i)];
  for (int j = 0; j < N; j++)
    f1[j] = f2[j];
}

void FFT(complex<double> *f, int N, int signo) //
{
  ordina(f, N); //first: reverse order
  complex<double> *W;
  W = (complex<double> *)malloc(N / 2 * sizeof(complex<double>));
  W[1] = polar(1., signo*2. * PI / N);
  W[0] = 1;
  for (int i = 2; i < N / 2; i++)
    W[i] = pow(W[1], i);
  int n = 1;
  int a = N / 2;
  for (int j = 0; j < log2(N); j++)
  {
    for (int i = 0; i < N; i++)
    {
      if (!(i & n))
      {
        complex<double> temp = f[i];
        complex<double> Temp = W[(i * a) % (n * a)] * f[i + n];
        f[i] = temp + Temp;
        f[i + n] = temp - Temp;
      }
    }
    n *= 2;
    a = a / 2;
  }
}

complex<double> tRF1C(int N, trackComplex entrada, int signo, int K)
{
    
    double theta = signo * 2 * PI * K;
    complex<double> aux, e, salida;
    trackComplex input;
    int mitad = N >> 1;
    double factor;
    int n = 2;

    input = new complex<double>[mitad];

    factor = (1 == signo) ? (double)1 / N : 1;

    while (1 <= mitad)
    {
        e.real(cos(theta / n));
        e.imag(sin(theta / n));
        for (int i = 0; i < mitad; i++)
        {
            if(mitad == N>>1){
              aux = entrada[i+mitad]*e;
              input[i] = entrada[i]+aux;
            }else
            {
              aux = input[i+mitad]*e;
              input[i] += aux;
            }
        }
        mitad >>= 1;
        n <<= 1;
    }

    salida = input[0] * factor;

    delete[] input;
    return salida;
}

void TRF(trackComplex entrada, int N, int signo)
{
    trackComplex p = new complex<double>[N];
    for (int K = 0; K < N; K++)
        p[K]=tRF1C(N, entrada, signo, K);
    for (int K = 0; K < N; K++)
        entrada[K] = p[K];
    delete[] p;
    
}

}
#endif