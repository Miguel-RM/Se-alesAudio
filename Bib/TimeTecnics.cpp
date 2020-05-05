#include <complex>
#include <iostream>
#include "estructuras.cpp"

using namespace::audio;

#ifndef TIMETEC_CPP
#define TIMETEC_CPP

/************************************************************************
 * en este espacio de nombres se guardan las funciones encargadas de    *
 * realizar el trabajo en el dominio del tiempo                         *
 * *********************************************************************/


namespace TimeTecnics{

/*
void AudioPart(wave *Audio, int framaSize, int advance){

    unsigned long int position, length;
    trackDouble track;

}*/

trackDouble Convolution(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel){
    

    int j0, j1, k, nk, n;
    trackDouble convol = new double[lenTrack];
    float suma;

    nk = lenKernel;
    n = lenTrack;

    for (int i = 0; i < n; i++)
    {
        suma = 0;
        j0 = i;
        j1 = i+nk;
        for (int j = j0; j <= j1 && j < n; j++)
        {
            suma += Track[k] * kernel[nk + i - j];
        }
        convol[i] = suma;
    }

    return convol;

}

trackDouble ConvolutionCirc(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel){
    
    int j0, j1, k, nk, n;
    trackDouble convol = new double[lenTrack];
    float suma;

    nk = lenKernel;
    n = lenTrack;

    for (int i = 0; i < n; i++)
    {
        suma = 0;
        j0 = i;
        j1 = i+nk;
        for (int j = j0; j <= j1; j++)
        {
            k = j + (j < 0 ? n : 0) - (j >= n ? n : 0);
            suma += Track[k] * kernel[nk + i - j];
        }
        convol[i] = suma;
    }

    return convol;
}

trackDouble Correlation(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel){

    int j0, j1, k, nk, n;
    trackDouble correl = new double[lenTrack];
    float suma;

    nk = lenKernel;
    n = lenTrack;

    for (int i = 0; i < n; i++)
    {
        suma = 0;
        j0 = i;
        j1 = i+nk;
        for (int j = j0; j <= j1 && j < n; j++)
        {
            suma += Track[k] * kernel[j - i];
        }
        correl[i] = suma;
    }

    return correl;
}

trackDouble CorrelationCirc(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel){
    int j0, j1, k, nk, n;
    trackDouble correl = new double[lenTrack];
    float suma;

    nk = lenKernel;
    n = lenTrack;

    for (int i = 0; i < n; i++)
    {
        suma = 0;
        j0 = i;
        j1 = i+nk;
        for (int j = j0; j <= j1; j++)
        {
            k = j + (j < 0 ? n : 0) - (j >= n ? n : 0);
            suma += Track[k] * kernel[nk + i - j];
        }
        correl[i] = suma;
    }

    return correl;
}

double EnergyTimeShort(trackInt chunk, int lenChunk){


    double energy = 0;
    for (int i = 0; i < lenChunk; i++)
    {
        energy +=  chunk[i]*chunk[i];
    }

    return energy;
}


double CreateHistogram(trackInt chunk){

return 0.0;
    
}

double EntropyTimeShort(trackInt chunk){
    
    return 0.0;
}

int signo(int muestra)
{
    return (muestra >= 0 ? 1 : -1);
}

double ZeroCrossing(trackInt chunk, int lenChunk){

    double cross = 0;

    for (int i = 1; i < lenChunk; i++)
    {
        cross += signo(chunk[i]) - signo(chunk[i-1]);
    }
    
    return cross;
}

}

#endif