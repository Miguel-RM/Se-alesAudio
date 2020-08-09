#include<iostream>
#include<cmath>
#include"estructuras.cpp"

using namespace::std;
using namespace::audio;

#ifndef WAVELET_CPP
#define WAVELET_CPP

namespace Wavelet
{

double searchMax(trackDouble track, int lengthT, int nivel)
{
    double max = 0, max2 = 0;
    int pS = lengthT >> nivel;
    
    max = abs(track[pS]);
    for (int i = pS; i < lengthT; i++)
    {
        if(max < abs(track[i])){
            
            max2 = max;
            max = abs(track[i]);
        }else if(max2 < abs(track[i]))
        {
            max2 = abs(track[i]);
        }
        
    }
    return max2;
}

void trimTrack(trackDouble track, int lengthT, double porC, int nivel)
{
    double max;
    double threshold;

    max = searchMax(track, lengthT, nivel);
    threshold = porC * max;

    cout << "Max: "  << max << endl;
    cout << "Umbral: "  << threshold << endl;

    for (int i = lengthT >> nivel; i < lengthT; i++)
    {
        if(abs(track[i])<threshold)
            track[i] = 0.0;
    }
}

index createIndex(matrizDouble tracks, int lengthT, int &lengthIndex, int numTracks)
{
    int k = 1, l = 0;
    index ind;
    unsigned char p = 128;
    int limit = numTracks*lengthT;
    int chann=0;

    lengthIndex = (int)ceil((numTracks*lengthT) / 8.0);
    cout << "LEnIndx " << lengthIndex << endl;
    cout << "limit "<< limit << endl;
    cout << "len "<< 576*8.0 << endl;

    ind = (unsigned char *)calloc(lengthIndex, sizeof(unsigned char));

    for (int i = 0, j = 0; i < limit; i++, k++)
    {
        if(i == lengthT)
        {
            i = 0;
            chann++;
        }
        if(abs(tracks[chann][i]) > 0.0)
            ind[l] = ind[l] | p;
        p >>= 1;
        if(8 == k){
            k = 0;
            l++;
            p = 128;
        }
        //cout << "i: " << i << " j " << " k " << k << " l "<< l << "P: " << (int)p<< endl;
    }
    return ind;
}


void updateInv(trackDouble &track, trackDouble &wave, int mitad, int nivel)
{
    if(1 == nivel)
    {
        delete[] track;
        track = wave;
    }else
    {
        for (int i = 0; i < mitad<<1; i++)
        {
            track[i] = wave[i];
        }
    }
}

trackDouble transDiscWavInv(trackDouble &track, int lengthT, trackDouble fHigh, trackDouble fLow, int lengthF, int nivel)
{
    int mitad;
    trackDouble wave = (double *)calloc(lengthT, sizeof(double));

    for (int i = nivel; i > 0 ; i--)
    {
        mitad = lengthT >> i;

        for (int n = 1; n <= mitad; n++)
        {   
            wave[2*n-2] = fHigh[0]*track[n-1] + fHigh[1]*track[n+mitad-1];
            wave[2*n-1] = fLow[0]*track[n-1] + fLow[1]*track[n+mitad-1];
        }
        updateInv(track, wave, mitad, i);
    }

    return wave;
}

void update(trackDouble &track, trackDouble &wave, int &lengthW, int &mitad, int nivel)
{
    if(0 == nivel)
    {
        delete[] track;
        track = wave;
    }else
    {
        for (int i = 0; i < lengthW; i++)
        {
            track[i] = wave[i];
        }
        delete[] wave;
        
    }
    lengthW = mitad;
    wave = (double *)calloc(lengthW, sizeof(double));
    if(mitad%2 != 0){
        wave[mitad-1] = track[mitad-1];
    }

    mitad >>= 1;
}

void transDiscWav(trackDouble &track, int lengthT, trackDouble fHigh, trackDouble fLow, int lengthF, int nivel)
{ 
    
    int mitad = lengthT >> 1;
    int lengthW;
    double low, high;
    trackDouble wave = (double *)calloc(lengthT, sizeof(double));

    for (int i = 0; i < nivel; i++)
    {
        for (int n = 1; n <= mitad; n++)
        {
            low = high = 0;
            for (int k = 0; k < lengthF; k++)
            {
                low += fLow[k] * track[2*n-k-1];
                high += fHigh[k] * track[2*n-k-1];
            }
            wave[n-1] = low;
            wave[n+mitad-1] = high;
        
        }
        update(track, wave, lengthW, mitad, i);
    }
}

}
#endif