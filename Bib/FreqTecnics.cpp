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


namespace FreqTecnics{

int BarkScale(double f)
{
  double factor;
  int banda;
  factor = f/7500;

  banda = 13 * atan(0.76 * f / 1000) + 3.5 * atan(factor * factor);

  return banda;
}

trackDouble Hamming(int N)
{
  
  trackDouble ventana = new double[N];
  for (int i = 0; i < N; i++)
  {
    ventana[i] = 0.53836 - 0.46164 * cos(2 * PI * i / (N - 1));
  }
  return ventana;
}

double CosDistance(trackDouble A, trackDouble B, int length)
{

  double Resultado, SuACu = 0.0, SuBCu = 0.0, SuAB = 0.0;

  for (int i = 0; i < length; i++)
  {

    SuAB += A[i] * B[i];
    SuACu += A[i] * A[i];
    SuBCu += B[i] * B[i];
  }

  SuACu = sqrt(SuACu);
  SuBCu = sqrt(SuBCu);

  Resultado = 1.0 - (SuAB / (SuACu * SuBCu));

  return Resultado;

}

double minkowskiDistance(trackDouble A, trackDouble B, int length)
{
  double SuAB = 0.0, aux;

  for (int i = 0; i < length; i++)
  {

    aux = A[i] - B[i];
    SuAB += aux * aux;
  }
  SuAB = sqrt(SuAB);
  return SuAB;
}

double ManhathaDistance(trackDouble A, trackDouble B, int length)
{
  double SuAB = 0.0;

  for (int i = 0; i < length; i++)
  {

    SuAB += fabs(A[i] - B[i]);
  }

  return SuAB;
}

}

#endif