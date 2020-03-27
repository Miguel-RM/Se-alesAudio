#include"estructuras.cpp"
#include<cmath>

#ifndef FILTER_CPP
#define FILTER_CPP

using namespace::audio;

namespace Filters
{
    trackDou gaussiana(double sigma, double media, int orden)
    {

        double a, c, aux, poli;
        int inf, sup;
        trackDou vec;
        a = 1 / (sigma * sqrt(2 * M_PI));
        c = 2 * pow(sigma, 2);
        inf = media - 3 * sigma;
        sup = media + 3 * sigma;

        for (int i = inf; i <= sup; i++)
        {
            aux = a * exp(((-1) * (i - media) * (i - media)) / c);
            switch (orden)
            {
            case 0:
                poli = 1;
                break;

            case 1:
                poli = -i / pow(sigma, 2);
                break;

            case 2:
                poli = pow(i, 2) / pow(sigma, 4) - 1 / pow(sigma, 2);
                break;

            default:
                poli = 1;
                break;
            }
            aux *= poli;
            vec.push_back(aux);
        }

        return vec;

        //6*sigma = tamaño
    }
}

#endif