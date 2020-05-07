#include <complex>
#include <iostream>
#include "estructuras.cpp"

using namespace::audio;

#ifndef FREQTEC_CPP
#define FREQTEC_CPP

/************************************************************************
 * en este espacio de nombres se guardan las funciones encargadas de    *
 * realizar el trabajo en el dominio de la frecuencia                   *
 * *********************************************************************/


namespace TimeTecnics{

void DTW()
{

}

trackDouble BarkScale()
{

}

trackDouble MFCC()
{

}

trackDouble Hamming(int N)
{
  int i = 0;
  trackDouble ventana = new double[N];
  for (; i < N; i++)
  {
    ventana[i] = 0.53836 - 0.46164 * cos(2 * PI * i / (N_MUESTRAS - 1));
  }
  return ventana;
}

void CosDistance()
{

}

void ManhathaDistance()
{

}

void minkowskiDistance()
{

}

}

#endif