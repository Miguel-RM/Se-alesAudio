#include<vector>
#include<complex>
using namespace::std;

#ifndef ESTRUCT_CPP
#define ESTRUCT_CPP
namespace audio{
    #define PI 3.14159265359
    #define Period 30e-3
    typedef vector<double> trackFloat;
    typedef vector<int> track32;
    typedef vector<short> track16;
    typedef vector<trackFloat> MatrixFloat;
    typedef vector<track32> Matrix32;
    typedef vector<track16> Matrix16;
    typedef double* trackDouble;
    typedef complex<double>* trackComplex;
    typedef double** matrizDouble;
    typedef complex<double>** matrizComplex;
    typedef int* trackInt;
    typedef int** matrizInt;
    typedef struct 
    {
        int v; // valor de la variable 
        int f; // frecuencoa de la variable
    } hist;  
    typedef vector<hist> Histogram;
    
}
#endif