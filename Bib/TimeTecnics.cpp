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

void Normal(int start, trackInt chunk, int lenChunk){

    double max = -1;
    int end = start + lenChunk;
    for (int i = start; i < end; i++)
    {
        if(chunk[i]> max)
            max = abs(chunk[i]);
    }

    for (int i = start; i < end; i++)
    {
        chunk[i] /= max;
    }
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

void insert(int value, int pos, Histogram &H)
{
    hist block;
    block.v = value;
    block.f = 1;
    H.insert(H.begin()+pos,block);
}

int BinarySearch(int value, Histogram &H)
{
    int start = 0;
    int it = 0;
    int end = H.size()-1;
    int middle = start + (end-start)/2;
    double aux;
    

    if (0 == H.size())
    {
        insert(value, 0, H);
        return 0;
    }
    
    while (true)
    {
        if(H[middle].v == value){
            H[middle].f++;
            break;
        }else if(value > H[middle].v)
        {
            if (middle == end) { insert(value, middle+1, H); break; }
            start= middle+1;
            aux = (double) (end-start)/2;
            middle = start + round(aux);
        }else if(value < H[middle].v)
        {
            if (middle == start) { insert(value, middle, H); break; }
            end = middle-1;
            aux = (end-start)/2;
            middle = start + aux;
        } it++;
    }  
    //cout << "iteraciones " << it << endl;
    return 0;  
}

Histogram CreateHistogram(int start, trackInt &chunk, int length){

    Histogram H;
    int end = start + length;

    for (int i = start; i < end; i++)
    {
        BinarySearch(chunk[i],H);
    }
    return H;
}

double EntropyTimeShort(int start, trackInt chunk, int length){
    
    Histogram H;
    double sum=0;
    double p,h;

    H = CreateHistogram(start, chunk, length);
    for (int i = 0; i < H.size(); i++)
    {
        p = H[i].f / (double)length;
        sum += p*log10(p);
    }
    h = (-1)*sum;
    return h;
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
        cross += abs(signo(chunk[i]) - signo(chunk[i-1]));
    }
    
    return cross;///(2*lenChunk);
}

int forwardSearch(double Emax, long lengthT, trackInt track, int framesize, int slide)
{

    double E = 0;
    double Weighing = 0;
    double thresholdE = Emax;
    double thresholdC = 9e18;
    double NoiseE = 0;
    double SNR = 0;
    double e,c;
    int cross = 0;
    int count = 0;

    for (int pos = 0; pos < lengthT; pos+=slide)
    {
        E = EnergyTimeShort(pos, track, framesize);
        cross = ZeroCrossing(pos, track, framesize);

        if(count < MTRASH)
        {
            if(NoiseE<E)
                NoiseE = E;
            if(thresholdC > cross)
                thresholdC = cross;
            count ++;
        }else if(count == MTRASH)
        {
            SNR = 10*log10(Emax/NoiseE);

            if(SNR < 20){
                NoiseE = Emax / pow(10, (SNR-3)/10);
            }else if(SNR < 30)
            {
                NoiseE = Emax / pow(10, (SNR-7)/10);
            }else
            {
                NoiseE = Emax / pow(10, 25/10);
            }
            //cout << "NoisEFor " << NoiseE << endl;
            count ++;
        }else
        {
            e = E > NoiseE ? 0.6 : 0;
            c = cross < thresholdC && cross < 15? 0.4 : 0;
            Weighing = e + c;
                   
        }
        if( Weighing >= 0.6)
             return pos;

    }

    return -1;
    
}

int backwardSearch(double Emax, long lengthT, trackInt track, int framesize, int slide)
{

    double E = 0;
    double Weighing = 0;
    double thresholdE = Emax;
    double thresholdC = 9e18;
    double NoiseE = 0;
    double SNR = 0;
    int cross = 0;
    int count = 0;
    double e,c;

    trackInt marco = new int[framesize];

    for (int pos = lengthT-1; pos > framesize; pos-=slide)
    {
        for (int i = 0; i < framesize; i++)
            marco[i] = track[pos - i];
        
        E = EnergyTimeShort(0, marco, framesize);
        cross = ZeroCrossing(0, marco, framesize);

        if(count < MTRASH)
        {
            if(NoiseE<E)
                NoiseE = E;
            if(thresholdC > cross)
                thresholdC = cross;
            count ++;
        }else if(count == MTRASH)
        {
            SNR = 10*log10(Emax/NoiseE);

            if(SNR < 20){
                NoiseE = Emax / pow(10, (SNR-3)/10);
            }else if(SNR < 30)
            {
                NoiseE = Emax / pow(10, (SNR-5)/10);
            }else
            {
                NoiseE = Emax / pow(10, 25/10);
            }
            //cout << "NoisEBac " << NoiseE << endl;
            //cout << "SNR " << SNR << endl;
            count ++;
        }else
        {
            e = E > NoiseE ? 0.6 : 0;
            c = cross < thresholdC && cross < 15? 0.4 : 0;
            Weighing = e + c;   
        }
        
        if( Weighing >= 0.6)
             return pos;

    }
    
    return -1;
}

int Segmenter(long lengthT, trackInt track, int framesize, int slide, int &start, int &end)
{

    double E = 0;
    double NoisE = 0;
    double Emax = -1;

    for (int pos = 0; pos + framesize < lengthT-1 ; pos+=slide)
    {
        E = EnergyTimeShort(pos, track, framesize);
        if(E > Emax)
            Emax = E;
    }

    start = forwardSearch(Emax, lengthT, track, framesize, slide);
    end   = backwardSearch(Emax, lengthT, track, framesize, slide);


    cout << "start: "<< start<< " end "<< end<< endl;
    if(-1 == start || -1 == end) return -1;
    
    return 1;
}

}

#endif