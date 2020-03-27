#include"estructuras.cpp"
#include<iostream>

#ifndef TRACKS_CPP
#define TRACKS_CPP

using namespace::audio;
using namespace::std;

namespace Tracks
{
    void printTrack(trackDou t)
    {
        cout << "track =[ ";
        for (int i = 0; i < t.size(); i++)
        {
            cout << t[i] << " ";
        }
        cout << " ];" << endl;
        
    }
    trackDou generaCeros(int samples)
    {
        trackDou reng;

        for (int i = 0; i < samples; i++)
        {
            reng.push_back(0.0);
        }

        return reng;
    }

    trackDou sumTrack(trackDou &A, trackDou &B)
    {
        trackDou c;

        if(A.size() != B.size())
        {
            cout << "the Tracks are of diferent length" << endl;
        }
        for (int i = 0; i < A.size(); i++)
        {
            c.push_back(A[i]+B[i]);
        }
        return c;
    }

    trackDou substracTrack(trackDou &A, trackDou &B)
    {
        trackDou c;

        if(A.size() != B.size())
        {
            cout << "the Tracks are of diferent length" << endl;
        }
        for (int i = 0; i < A.size(); i++)
        {
            c.push_back(A[i]-B[i]);
        }
        return c;
    }

    trackDou multTrack(trackDou &A, trackDou &B)
    {
        trackDou c;

        if(A.size() != B.size())
        {
            cout << "the Tracks are of diferent length" << endl;
        }
        for (int i = 0; i < A.size(); i++)
        {
            c.push_back(A[i]*B[i]);
        }
        return c;
    }

    void multTK(trackDou &T, trackDou &K, int desp)
    {
        trackDou c;
        int up = K.size()+desp;

        if(T.size() < up)
        {
            cout << "the desplacement are huge" << endl;
        }
        for (int i = 0; i < T.size(); i++)
        {
            if(desp<=i && i< up){
                T[i] *= K[i-desp];
            }else{
                T[i] = 0.0;
            }
        }
        
    }
}

#endif