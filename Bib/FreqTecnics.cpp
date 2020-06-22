#include <complex>
#include <iostream>
#include "estructuras.cpp"

using namespace ::audio;

#ifndef FREQTEC_CPP
#define FREQTEC_CPP

/************************************************************************
 * en este espacio de nombres se guardan las funciones encargadas de    *
 * realizar el trabajo en el dominio de la frecuencia                   *
 * *********************************************************************/

namespace FreqTecnics
{

  int BarkScale(double f)
  {
    double factor;
    int banda;
    factor = f / 7500;

    banda = 13 * atan(0.76 * f / 1000) + 3.5 * atan(factor * factor);

    return banda;
  }

  trackDouble bandsBark(int frameSize, trackComplex marco, double SamplesPerSec, double &max, double &min)
  {
    int banda;
    double energy, magnitude;
    double bandwith = SamplesPerSec / frameSize;
    trackDouble bark;

    bark = (double *)calloc(BANDS, sizeof(double));
    for (int i = 0; i < frameSize / 2; i++)
    {
      magnitude = abs(marco[i]);
      energy = magnitude * magnitude;
      banda = BarkScale(i * bandwith);

      if (banda < BANDS)
        bark[banda] += energy;
      else
        break;
    }

    for (int i = 0; i < BANDS; i++)
    {
      if (bark[i] > max)
          max = bark[i];
      if (bark[i] < min)
          min = bark[i];
    }

    return bark;
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

  double EuclideanDistance(trackDouble A, trackDouble B, int length)
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

} // namespace FreqTecnics

#endif