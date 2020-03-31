#include"estructuras.cpp"
#include<iostream>

#ifndef TRACKS_CPP
#define TRACKS_CPP

using namespace::audio;
using namespace::std;

namespace Tracks
{
    void printTrack(trackFloat t)
    {
        cout << "track =[ ";
        for (int i = 0; i < t.size(); i++)
        {
            cout << t[i] << " ";
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

    trackFloat substracTrack(trackFloat &A, trackFloat &B)
    {
        trackFloat c;

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

    trackFloat multTrack(trackFloat &A, trackFloat &B)
    {
        trackFloat c;

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

    void multTK(trackFloat &T, trackFloat &K, int desp)
    {
        
        int threesigma = K.size()/2;
        trackFloat aux = generaCeros(T.size());

        int up = threesigma+desp;
        int down = desp-threesigma;
        int max = T.size()-1;
        cout << "Framesize: "<< T.size()<<endl;

        if(T.size()/2 < desp)
        {
            cout << "the desplacement are huge" << endl;
            exit(0);
        }

        for (int j=0,i = desp-threesigma; i <= desp+threesigma; i++,j++)
        {
            if(i>=0&&i<T.size()/2){
                aux[i] = T[i]*K[j];
                aux[max-i] = T[max-i]*K[j];
               
            }
            
        }
        T=aux;

    }
}

#endif