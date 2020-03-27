#include "estructuras.cpp"
#include<cmath>

using namespace::audio;

#ifndef FOURIER_CPP
#define FOURIER_CPP

namespace Fourier{

void tRF1C(trackDou entradaR, trackDou entradaI, double &salidaR, double &salidaI, int signo, int K)
{
    int tam = entradaR.size();
    int mitad = tam / 2;
    int N = 2;
    double theta = signo * 2 * PI * K;
    double eR, eI, real, imag, factor;

    factor = (1 == signo) ? (double)1 / tam : 1;

    while (1 <= mitad)
    {
        eR = cos(theta / N);
        eI = sin(theta / N);
        for (int i = 0; i < mitad; i++)
        {
            real = entradaR[i + mitad] * eR - entradaI[i + mitad] * eI;
            imag = entradaR[i + mitad] * eI + entradaI[i + mitad] * eR;

            entradaR[i] += real;
            entradaI[i] += imag;
        }
        mitad /= 2;
        N *= 2;
    }

    salidaR = entradaR[0] * factor;
    salidaI = entradaI[0] * factor;
}

void tRF1D(trackDou &entradaR, trackDou &entradaI, trackDou &salidaR, trackDou &salidaI, int signo)
{
    int tam = entradaR.size();

    for (int K = 0; K < tam; K++)
        
        tRF1C(entradaR, entradaI, salidaR[K], salidaI[K], signo, K);
}


}
#endif