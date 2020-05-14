#include <complex>
#include <iostream>
#include "estructuras.cpp"

using namespace::audio;

#ifndef HMMPC_CPP
#define HMMPC_CPP

/************************************************************************
 * en este espacio de nombres se guardan las funciones encargadas de    *
 * realizar el trabajo Homomorfico                                      *
 * *********************************************************************/


namespace Homomorphic{


void FilterTriang(int num)
{

}

trackDouble MelScale(double f)
{
    return 2595*log10((f/700)+1);
}

trackDouble MFCC()
{

}

}

#endif