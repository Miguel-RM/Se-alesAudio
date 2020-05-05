#include"estructuras.cpp"
#include"tracks.cpp"
#include<cmath>

#ifndef FILTER_CPP
#define FILTER_CPP

using namespace::audio;
using namespace::Tracks;

namespace Filters
{
    void aplyKernel(int marcos, int framesize, int sigma, trackDouble kernel, matrizComplex spectre, int desp)
    {
        for (int  i = 0; i < marcos; i++)
        {
            SpectrepuntKern(framesize, sigma, spectre[i], kernel, desp);
        }
        
    }
    
    trackDouble gaussiana(int sigma, double media)
    {

        double a, c, aux, poli, Max;
        int inf, sup, max, length;
        trackDouble vec;
        a = 1 / (sigma * sqrt(2 * M_PI));
        max = 3*sigma;
        c = 2 * pow(sigma, 2);
        inf = media - 3 * sigma;
        sup = media + 3 * sigma;
        length = 6*sigma +1;
        vec = new double[length];

        for (int i = inf, j=0; i <= sup; i++, j++)
        {
            aux = a * exp(((-1) * (i - media) * (i - media)) / c);

            vec[j]=aux;
            //cout << aux << " ";

        }//cout<< endl;
        cout << sigma << " d"<<endl;
        Max = vec[max]; 
        for (int i = 0; i < length; i++)
        {
            vec[i] /= Max;
        }
        

        return vec;

        //6*sigma = tamaño
    }
}

#endif