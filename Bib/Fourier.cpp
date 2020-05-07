#include "estructuras.cpp"
#include <complex.h>
#include <fftw3.h>
#include <cmath>

using namespace ::audio;

#ifndef FOURIER_CPP
#define FOURIER_CPP

namespace Fourier
{

/**********************************************
* Funciones independientes.                   *
*                                             *
***********************************************/

int log2(int N)
{
  int k = N, i = 0;
  while (k)
  {
    k >>= 1;
    i++;
  }
  return i - 1;
}

int reverse(int N, int n)
{
  int j, p = 0;
  for (j = 1; j <= log2(N); j++)
  {
    if (n & (1 << (log2(N) - j)))
      p |= 1 << (j - 1);
  }
  return p;
}

void ordina(complex<double> *f1, int N)
{
  complex<double> f2;
  int rev;

  for (int i = 0; i < N / 2; i++)
  {
    rev = reverse(N, i);
    f2 = f1[i];
    f1[i] = f1[rev];
    f1[rev] = f2;
  }
}

void FFT(complex<double> *f, int N, int signo) //
{
  ordina(f, N);
  long mitad = N >> 1;
  complex<double> *W;
  complex<double> temp, Temp;
  W = new complex<double>[mitad];
  W[1] = polar(1., signo * 2. * PI / N);
  W[0] = 1;
  for (long i = 2; i < mitad; i++)
    W[i] = pow(W[1], i);
  long n = 1;
  long a = mitad;

  for (long j = 0; j < log2(N); j++)
  {
    for (long i = 0; i < N; i++)
    {
      if (!(i & n))
      {
        temp = f[i];
        Temp = W[(i * a) % (n * a)] * f[i + n];
        f[i] = temp + Temp;
        f[i + n] = temp - Temp;
      }
    }
    n <<= 1;
    a >>= 1;
  }
}

void TRFW(trackComplex f, int N, int signo)
{
  fftw_complex *in, *out;
  fftw_plan p;

  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * N);

  for (int i = 0; i < N; i++)
  {
    in[i][0] = f[i].real();
    in[i][1] = f[i].imag();
  }

  p = fftw_plan_dft_1d(N, in, out, signo, FFTW_ESTIMATE);
  fftw_execute(p);

  for (int i = 0; i < N; i++)
  {
    if(-1==signo){
      f[i].real(out[i][0]);
      f[i].imag(out[i][1]);
    }else
    {
      f[i].real(out[i][0]/N);
      f[i].imag(out[i][1]/N);
    }
    
  }

  fftw_execute(p);
  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
}

} // namespace Fourier
#endif