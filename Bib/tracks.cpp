#include "estructuras.cpp"
#include <iostream>

#ifndef TRACKS_CPP
#define TRACKS_CPP

using namespace ::audio;
using namespace ::std;

namespace Tracks
{

void printTrack(trackDouble t, long length)
{
    cout << "track =[ ";
    for (long i = 0; i < length; i++)
    {
        cout << t[i] << " ";
    }
    cout << " ];" << endl;
}

void printTrackC(trackComplex t, long length)
{
    cout << "track =[ ";
    for (long i = 0; i < length; i++)
    {
        cout << t[i].real() << " ";
    }
    cout << " ];" << endl;
}

trackFloat generaCeros(int samples)
{
    trackFloat reng;

    for (int i = 0; i < samples; i++)
    {
        reng.push_back(0.0);
    }

    return reng;
}

trackFloat sumTrack(trackFloat &A, trackFloat &B)
{
    trackFloat c;

    if (A.size() != B.size())
    {
        cout << "the Tracks are of diferent length" << endl;
    }
    for (int i = 0; i < A.size(); i++)
    {
        c.push_back(A[i] + B[i]);
    }
    return c;
}

trackFloat substracTrack(trackFloat &A, trackFloat &B)
{
    trackFloat c;

    if (A.size() != B.size())
    {
        cout << "the Tracks are of diferent length" << endl;
    }
    for (int i = 0; i < A.size(); i++)
    {
        c.push_back(A[i] - B[i]);
    }
    return c;
}

trackFloat multTrack(trackFloat &A, trackFloat &B)
{
    trackFloat c;

    if (A.size() != B.size())
    {
        cout << "the Tracks are of diferent length" << endl;
    }
    for (int i = 0; i < A.size(); i++)
    {
        c.push_back(A[i] * B[i]);
    }
    return c;
}

void SpectrepuntKern(int framesize, int kernelsize, trackComplex T, trackDouble K, int desp)
{

    int mitad = (kernelsize/2);
    int up = desp + mitad;
    int frecMax = framesize>>1;
    int i = 0;

    desp -= mitad;
    framesize--;

    if(desp<0){ i = - desp; desp += abs(desp); } 

    //cout << "Desplazamiento: "<<frecMax<<" "<<down<<endl;

    for (int j = 0; j < frecMax; j++)
    {
        if(j==desp && j<up){
            T[j] *= K[i];
            T[framesize-j] *= K[i];
            i++;
            desp++;
        }else
        {
            T[j]=0;
            T[framesize-j]=0;
        }
    }
}
} // namespace Tracks

#endif