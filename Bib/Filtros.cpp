#include"estructuras.cpp"
#include"tracks.cpp"
#include<cmath>

#ifndef FILTER_CPP
#define FILTER_CPP

using namespace::audio;
using namespace::Tracks;

namespace Filters
{

    /*************************************************************************************
     *                                  Filtros IIR                                      *
     * **********************************************************************************/

    double Min(double A, double B)
    {
        if(A<B)
            return A;
        else
            return B;
    }

    void FiltrosButterworth(double omega1, double omega2, double omegaL, double omegaU, double k1, double k2)
    {
        double o1 = 2*PI * omega1;
        double o2 = 2*PI * omega2;
        double oL = 2*PI * omegaL;
        double oU = 2*PI * omegaU;
        double A, B, oR, n;

        A = (o1*o1 + oL*oU) / (o1*(oL-oU));
        cout << "A: "<<A << " ";
        B = (o2*o2 + oL*oU) / (o2*(oU-oL));
        cout << "B: "<<B << " ";

        oR = Min(abs(A), abs(B));
        cout << "oR: "<<oR << " ";
        n = log10( (pow(10, (-k1/10))-1) /(pow(10, (-k1/10))-1) ) / (2*log10(1/oR));
        cout << "n: "<<n << " " << endl;


    }


    /*************************************************************************************
     *                                  Filtros FIR                                      *
     * **********************************************************************************/

    int oneRaisedK(int k)
    {
        if(0 == k%2)
            return 1;
        else 
            return -1;
    }

    bool par(int M)
    {   
        if(0==M%2)
            return true;
        else 
            return false;

    }

    trackFloat coefFIR(Hr &dataHr, int M)
    {
        int length = (dataHr[3].k - dataHr[0].k)+1, U, k, j;
        double sum, cons = PI / M;
        trackDouble G = new double[length];
        trackDouble h;
        trackFloat hf;

        if(par(M))
            U = M/2 -1;
        else 
            U = (M-1)/2;

        h = (double *)calloc(U+1, sizeof(double));

        for (int k = dataHr[1].k, i=1; k <= dataHr[2].k; k++, i++)
        {
            G[i] = oneRaisedK(k);
        }
        G[0] = oneRaisedK(dataHr[0].k)*dataHr[0].h;
        G[length-1] = oneRaisedK(dataHr[3].k)*dataHr[3].h;

        for (int n = 0; n <= U; n++)
        {
            sum = 0.0;
            k = 0==dataHr[0].k ? 1 : dataHr[0].k;
            j = 0==dataHr[0].k ? 1 : 0;
            for (; j < length; j++, k++)
            {
                sum += G[j]*cos(cons*(2*n+1)*k);
            }
            if(0==dataHr[0].k)
                sum = G[0] + 2*sum;
            else 
                sum *= 2;
            h[n] = sum / M;   
        }

        for (int i = 0; i < M/2; i++)
        {
            hf.push_back(h[i]);   
        }

        for (int i = 0; i < M/2; i++)
        {
            hf.push_back(h[M/2-i-1]);   
        }
        delete[] h;
        
        return hf;
    }

    /*************************************************************************************
     *         Obtiene la y actual con base en los paramentros de entrada                *
     * **********************************************************************************/

    void output(int n, int outputs, trackFloat a, trackFloat b,trackInt track, trackDouble y)
    {
       
        double auxb = 0, auxa = 0;
        for (int i = 0; i < b.size(); i++)
        {
            auxb += b[i]*track[n - i];
        } 

        for (int i = 1; i < a.size(); i++)
        {
            if(outputs > a.size()-1)
            {
                auxa += a[i-1]*y[n - i];
                outputs--;
            }
        }
        
        y[n] = auxa + auxb;
    }

    /*************************************************************************************
     *              Obtiene la señal filtrada con el filtro m                            *
     * **********************************************************************************/


    void applyFilter(int lengthT, trackFloat a, trackFloat b, trackInt track, trackDouble y)
    {
        
        int cont=0;
        for (int n = 0; n < lengthT; n++)
        {
            if(n<=b.size())
            {
                y[n] = (double)track[n];
            }else
            {
                output(n, cont, a, b, track, y);
                cont++;
            }   
        }
    }

    /*************************************************************************************
     *         Mezcla todas las señales filtradas ponderando cada una de ellas           *
     * **********************************************************************************/


    trackDouble Mixer(long lengthT, trackInt track, MatrizDouble bankA, MatrizDouble bankB, trackDouble weighing)
    {

        trackDouble mix = (double *)calloc(lengthT, sizeof(double));
        trackDouble y   = new double[lengthT];
        double fac = 1;
 
        for (int i = 0; i < bankB.size(); i++)
        {
            cout <<endl << "\t Aplicando Filtro " << i+1 << endl;
            applyFilter(lengthT, bankA[i], bankB[i], track, y);
            for (int n = 0; n < lengthT; n++)
            {
                mix[n] += weighing[i] * fac * y[n];
            }
        }
        delete[] y;
        
        return mix;
    }
    
    trackDouble gaussiana(int sigma, double media)
    {

        double a, c, aux, poli, Max;
        int inf, sup, max, length;
        trackDouble vec;
        a = 1 / (sigma * sqrt(2 * M_PI));
        max = 3*sigma;
        c = 2 * pow(sigma, 2);
        inf = media - 3 * sigma;
        sup = media + 3 * sigma;
        length = 6*sigma +1;
        vec = new double[length];

        for (int i = inf, j=0; i <= sup; i++, j++)
        {
            aux = a * exp(((-1) * (i - media) * (i - media)) / c);

            vec[j]=aux;
            //cout << aux << " ";

        }//cout<< endl;
        cout << sigma << " d"<<endl;
        Max = vec[max]; 
        for (int i = 0; i < length; i++)
        {
            vec[i] /= Max;
        }
        

        return vec;

        //6*sigma = tamaño
    }
}

#endif