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
        with = hz2Mel(samplePerSec) / (FILTERS + 1);

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

    trackDouble cosTrans(trackDouble filters, int coef)
    {

        trackDouble mfcc;
        double aux;

        mfcc = new double[coef];
        for (int i = 0; i < coef; i++)
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

    trackDouble MFCC(trackComplex marco, int framesize, int samplesPerSeg)
    {

        trackDouble fc;
        double bandWith = samplesPerSeg / framesize;
        trackDouble filters = (double *)calloc(FILTERS, sizeof(double));
        trackDouble mfcc;
        int coef = 8;

        fc = createFc(samplesPerSeg);
        for (int i = 0; i < framesize / 2; i++)
        {
            for (int m = 0; m < FILTERS; m++)
            {
                filters[m] += H(bandWith * i, fc, m + 1) * abs(marco[i]);
            }
        }

        for (int i = 0; i < FILTERS; i++)
        {
            filters[i] = log10(filters[i]);
        }
        
        mfcc = cosTrans(filters, coef);

        delete[] fc;
        delete[] filters;

        return mfcc;
    }

} // namespace Homomorphic

#endif