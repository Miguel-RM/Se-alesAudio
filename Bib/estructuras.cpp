#include<vector>
#include<complex>
using namespace::std;

#ifndef ESTRUCT_CPP
#define ESTRUCT_CPP
namespace audio{
    #define PI 3.14159265359
    #define PERIOD 30e-3
    #define SLICE 10e-3
    #define MTRASH 8
    #define FILTERS 15
    #define BANDS 16
    #define PATH "AudiosIn/"
    #define PATHDICT "dictionary/"
    
    typedef vector<double> trackFloat;
    typedef double* trackDouble;
    typedef complex<double>* trackComplex;
    typedef vector<trackFloat> MatrizDouble;
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

    typedef struct
    {
        double h; // energia 
        int k; // K
    } hr;
    typedef vector<hr> Hr;
    
}
#endif