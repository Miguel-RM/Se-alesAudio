#include <complex>
#include <iostream>
#include "estructuras.cpp"

using namespace ::audio;

#ifndef HMMPC_CPP
#define HMMPC_CPP

/************************************************************************
 * en este espacio de nombres se guardan las funciones encargadas de    *
 * realizar el trabajo Homomorfico                                      *
 * *********************************************************************/

namespace Homomorphic
{

    double hz2Mel(double f)
    {
        return 2595.0 * log10((f / 700.0) + 1);
    }

    double mel2Hz(double mel)
    {
        return 700 * (pow(10, mel / 2595.0) - 1);
    }

    trackDouble createFc(int samplePerSec)
    {
        trackDouble fc;
        double with;
        fc = (double *)calloc(FILTERS + 2, sizeof(double));
        with = hz2Mel(samplePerSec/2) / (FILTERS + 1);

        for (int i = 0; i < FILTERS + 2; i++)
        {
            fc[i] = mel2Hz(i * with);
        }

        return fc;
    }

    double H(double f, trackDouble fc, int m)
    {
        if (f < fc[m - 1])
        {
            return 0.0;
        }
        if (fc[m - 1] <= f && f < fc[m])
        {
            return (f - fc[m - 1]) / (fc[m] - fc[m - 1]);
        }
        if (fc[m] <= f && f < fc[m + 1])
        {
            return (f - fc[m + 1]) / (fc[m] - fc[m + 1]);
        }

        return 0.0;
    }

    trackDouble cosTrans(trackDouble filters)
    {

        trackDouble mfcc;
        double aux;

        mfcc = new double[MCOEF];
        for (int i = 0; i < MCOEF; i++)
        {
            aux = 0.0;
            for (int j = 0; j < FILTERS; j++)
            {
                aux += filters[j] * cos((j - 0.5) * (PI * i / FILTERS));
            }
            mfcc[i] = aux;
        }

        return mfcc;
    }

    trackDouble MFCC(int framesize, trackComplex marco, int samplesPerSeg, double &max, double &min)
    {

        trackDouble fc;
        double bandWith = samplesPerSeg / framesize;
        double h;
        trackDouble filters = (double *)calloc(FILTERS, sizeof(double));
        trackDouble mfcc;

        fc = createFc(samplesPerSeg); // se calculan las frecuencias centrales
        for (int i = 0; i < framesize / 2; i++)
        {
            for (int m = 0; m < FILTERS; m++)
            {
                h = H(bandWith * i, fc, m + 1);
                filters[m] += h * abs(marco[i]);
            }
        }

        for (int i = 0; i < FILTERS; i++)
        {
            filters[i] = log10(filters[i]);
        }

        mfcc = cosTrans(filters);
        
        //cout << "MFCC: ";
        for (int i = 0; i < MCOEF; i++)
        {
            //cout << mfcc[i] <<  " ";
            if (mfcc[i] > max)
                max = mfcc[i];
            if (mfcc[i] < min)
                min = mfcc[i];
        }//cout << endl;

        delete[] fc;
        delete[] filters;

        return mfcc;
    }

} // namespace Homomorphic

#endif