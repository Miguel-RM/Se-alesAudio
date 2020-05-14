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

double EnergyTimeShort(int start, trackInt chunk, int lenChunk){


    double energy = 0;
    int end = start + lenChunk;
    for (int i = start; i < end; i++)
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

int ZeroCrossing(int start, trackInt chunk, int lenChunk){

    int cross = 0;
    int end = start + lenChunk;
    for (int i = start + 1; i < lenChunk; i++)
    {
        cross += signo(chunk[i]) - signo(chunk[i-1]);
    }
    
    return cross;
}

int forwardSearch(long lengthT, trackInt track, double weight[], double threshold[], int framesize, int slide)
{

    double E = 0;
    double Weighing = 0;
    double Entro= 0;
    int cross = 0;

    for (int pos = 0; pos < lengthT; pos+=slide)
    {
        E = EnergyTimeShort(pos, track, framesize);
        cross = ZeroCrossing(pos, track, framesize);
        Weighing = E>threshold[0] ? weight[0] : 0 + cross<threshold[1]? weight[1] : 0 + Entro*weight[2];

        if( Weighing > threshold[3]) return pos;

    }

    return -1;
    
}

int backwardSearch(long lengthT, trackInt track, double weight[], double threshold[], int framesize, int slide)
{

    double E = 0;
    double Weighing = 0;
    double Entro= 0;
    int cross = 0;

    trackInt marco = new int[framesize];

    for (int pos = lengthT-1; pos > framesize; pos-=slide)
    {
        for (int i = 0; i < framesize; i++)
            marco[i] = track[pos - i];
        
        E = EnergyTimeShort(0, marco, framesize);
        cross = ZeroCrossing(0, marco, framesize);
        Weighing = E>threshold[0] ? weight[0] : 0 + cross<threshold[1]? weight[1] : 0 + Entro*weight[2]; 

        if( Weighing > threshold[3]) return pos;

    }

    return -1;
}

int Segmenter(long lengthT, trackInt track, int framesize, int slide, int &start, int &end)
{
    double weight[3] = {0.6,0.4,0.0};
    double threshold[4];

    threshold[0] = EnergyTimeShort(0, track, lengthT)*1e-3;
    threshold[1] = 35;
    threshold[2] = 0;
    threshold[3] = 0.5;
    start = forwardSearch(lengthT, track, weight,threshold, framesize, slide);
    end   = backwardSearch(lengthT, track, weight,threshold, framesize, slide);

    if(-1 == start || -1 == end) return -1;
    
    return 1;
}

}

#endif