#include<vector>
using namespace::std;

#ifndef ESTRUCT_CPP
#define ESTRUCT_CPP
namespace audio{
    #define PI 3.14159265359
    typedef vector<double> trackFloat;
    typedef vector<int> track32;
    typedef vector<short> track16;
    typedef vector<trackFloat> MatrixFloat;
    typedef vector<track32> Matrix32;
    typedef vector<track16> Matrix16;
}
#endif