#include <complex>
#include <iostream>
#include "estructuras.cpp"
#include "Wave.cpp"

using namespace ::audio;

#ifndef MISCELLAN_CPP
#define MISCELLAN_CPP

namespace Miscellan
{

    

    void readWaveBin(Wave &audio, string name)
    {
    }

    void saveLPCBin(string name, vector<bool> vNv, vector<double> tono, vector<double> VRMS, vector<trackDouble> lpc_coef)
    {
    }

    void readBin(string name, vector<bool> vNv, vector<double> tono, vector<double> VRMS, vector<trackDouble> lpc_coef)
    {
    }
} // namespace Miscellan

#endif