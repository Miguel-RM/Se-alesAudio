#include <complex>
#include <cstdlib>
#include <iostream>
#include "estructuras.cpp"

using namespace ::audio;

#ifndef TIMETEC_CPP
#define TIMETEC_CPP

/************************************************************************
 * en este espacio de nombres se guardan las funciones encargadas de    *
 * realizar el trabajo en el dominio del tiempo                         *
 * *********************************************************************/

namespace TimeTecnics
{

    trackDouble preEnfasis(trackInt track, int sizeT)
    {

        trackDouble y;
        double a = 0.98;

        y = new double[sizeT];
        y[0] = track[0];

        for (int i = 1; i < sizeT; i++)
        {
            y[i] = track[i] - a * track[i - 1];
        }

        return y;
    }

    trackDouble Hamming(int N)
    {

        trackDouble ventana = new double[N];
        for (int i = 0; i < N; i++)
        {
            ventana[i] = 0.53836 - 0.46164 * cos(2 * PI * i / (N - 1));
        }
        return ventana;
    }

    int signo(double muestra)
    {
        return (muestra >= 0 ? 1 : -1);
    }

    int ZeroCrossing(int start, trackDouble chunk, int lenChunk)
    {

        int cross = 0;
        int end = start + lenChunk;
        for (int i = start + 1; i < lenChunk; i++)
        {
            cross += abs(signo(chunk[i]) - signo(chunk[i - 1]));
        }

        return cross/2;
    }


    double EnergyTimeShort(int start, trackInt chunk, int lenChunk)
    {

        double energy = 0;
        int end = start + lenChunk;
        for (int i = start; i < end; i++)
        {
            energy += chunk[i] * chunk[i];
        }

        return energy;
    }

    double rMS(int start, trackDouble track, int lenChunk)
    {

        double energy = 0;
        int end = start + lenChunk;
        for (int i = start; i < end; i++)
        {
            energy += track[i] * track[i];
        }
        energy /= lenChunk;

        return sqrt(energy);
    }

    double trimtoCenter(int n, int marcsize, trackDouble track)
    {
        int tercio = marcsize / 3;
        int start = n * marcsize;
        int end = start + marcsize;
        double IPK1 = 0, IPK2 = 0;

        for (int i = 0; i < tercio; i++)
        {
            if (abs(track[start + i]) > IPK1)
                IPK1 = abs(track[start + i]);
            if (abs(track[end - i]) > IPK2)
                IPK2 = abs(track[end - i]);
        }

        if (IPK1 > IPK2)
            return IPK2;

        return IPK1;
    }

    trackDouble autoCorrMod(int n, int marcsize, trackDouble track)
    {
        int pos = n * marcsize;
        int end = pos + marcsize;
        int desp;
        trackDouble autocorr;
        double CL, aux1, aux2;
        autocorr = (double *)calloc(marcsize, sizeof(double));
        CL = trimtoCenter(n, marcsize, track);
        CL *= 0.7;

        for (int k = 0; k < marcsize; k++)
        {
            for (int i = pos; i < end; i++)
            {
                desp = i + k;
                if(desp > end) break;
                //if(desp > end) desp = pos + (desp-end-1);
                aux1 = track[i];
                aux2 = track[desp];

                aux1 = abs(aux1) <= CL ? 0.0 : aux1 < 0?  aux1 + CL : aux1 - CL;
                aux2 = abs(aux2) <= CL ? 0.0 : aux2 < 0?  aux2 + CL : aux2 - CL;

                autocorr[k] += aux1 * aux2;
            }
        }

        return autocorr;
    }

    void searchPeak(trackDouble autocorr, int size, int &IPOS, double &IPK)
    {

        IPOS = -1;
        IPK = 0.0;

        for (int i = 1; i < size; i++)
        {
            if (autocorr[i] > IPK && autocorr[i] > autocorr[i-1] && autocorr[i] > autocorr[i+1])
            {
                IPK = autocorr[i];
                IPOS = i;
            }
        }
    }

    trackDouble getMarco(trackDouble track, trackDouble windows, int marcSize, int i)
    {
        int end, start;
        trackDouble marco;

        start = i*80;
        end = start + marcSize;
        marco = new double[marcSize];

        for (int i = start, j=0; i < end; i++,j++)
        {
            marco[j] = track[i]*windows[j];
        }

        return marco;
    }

    void vocNovoc(bool &vnv, double &tono, double &G, int n, int marcsize, trackDouble track, trackDouble &R, trackDouble windows, double fm, int lpcC)
    {
        int IPOS, zc;
        double IPK;
        trackDouble aC, marco;
        
        marco = getMarco(track, windows, marcsize, n);
        R = new double[lpcC+1];
        zc = ZeroCrossing(0, marco, marcsize);
        aC = autoCorrMod(0, marcsize, marco);
        searchPeak(aC, marcsize, IPOS, IPK);
        G = rMS(n*80, track, marcsize);

        cout << "ZC: "<<zc << endl;
        cout << "IPOS: "<<IPOS << endl;
        cout << "IPK: "<< IPK << " AC "<<aC[0]* 0.1<<endl;
        
        //(-7.8e-4*IPOS+0.4)
        if (IPOS < 9 || zc > 100)
        {
            vnv = false;
            tono = 0;
        }
        else
        {
            vnv = true;
            tono = ((double)IPOS) / fm;
        }

        for (int i = 0; i <= lpcC; i++)
        {
            R[i] = aC[i];
        }

        delete[] aC;
    }

    void detQAB(int k, int n, int &q, int &a, int &b)
    {
        if (k)
            q = n / 2 * k;
        else
            cout << "divición por cero" << endl;
        if ((2 * q + 1) * k <= n)
            a = 1;
        else
            a = 0;
        b = n - 2 * q * k - a * k;
    }

    double blakenship(int marcsize, int k, trackDouble x)
    {
        int q, a, b, t, ti;
        double b_k = 0, c_k = 0, s;

        detQAB(k, marcsize, q, a, b);
        for (int j = 0; j < q; j++)
        {
            s = 0;
            t = 2 * j * k;
            for (int i = 1; i <= k; i++)
            {
                ti = t + i;
                s += x[ti + k] * (x[ti] + x[ti + 2 * k]);
            }
            b_k += s;
        }
        t = 2 * q * k;
        if (a == 0)
        {
            for (int i = 1; i <= b; i++)
            {
                ti = t + i;
                c_k += x[ti] * x[ti + k];
            }
        }
        else
        {
            for (int i = 1; i <= b; i++)
            {
                ti = t + i;
                c_k += x[ti + k] * (x[ti] + x[ti + 2 * k]);
            }
            for (int i = b + 1; i <= k; i++)
            {
                ti = t + i;
                c_k += x[ti] * x[ti + k];
            }
        }
        return b_k + c_k;
    }

    trackDouble cLPC(int p, trackDouble R)
    {
        double s;
        trackDouble E;
        trackDouble k, lpc;
        matrizDouble alpha;

        E = new double[p];
        k = new double[p];
        alpha = new double*[p];
        E[0] = R[0];

        for (int i = 1; i <= p; i++)
        {
            s = 0;
            alpha[i-1] = new double[i];
            for (int j = 1; j < i; j++)
            {
                s += alpha[i-2][j-1] * R[i-j];
            }

            k[i-1] = (R[i] - s)/E[i-1];
            //cout << "k: " << k[i-1] << endl;
            alpha[i-1][i-1] = k[i-1];

            for (int j = 1; j < i; j++)
            {
                alpha[i-1][j-1] = alpha[i-2][j-1] - k[i-1]*alpha[i-2][(i-1)-j];
            }
            
            if(i<p) E[i] = (1-k[i-1]*k[i-1])*E[i-1];
            //cout << "E: "<< E[i] << endl;
            
        }

        lpc = alpha[p-1];
        for (int i = 0; i < p-1; i++)
        {
            delete[] alpha[i];
        }
        
        delete[] alpha;
        delete[] k;
        delete[] E;

        return lpc;
    }

    trackDouble pulseGenerator(int period, int marcsize)
    {
        trackDouble train;
        train = (double *)calloc(marcsize, sizeof(double));

        for (int i = 0; i < marcsize;)
        {
            train[i] = 1.0;
            i+=period;
        }   

        return train;
    }

    trackDouble noiseGenerator(int marcsize)
    {

        trackDouble train;
        train = new double[marcsize];
        srand48(time(NULL)); 

        for (int i = 0; i < marcsize; i++)
        {
            train[i] = drand48()*2.0-1.0;
        }   

        return train;
    }

    trackDouble voiceSynthesizer(bool vNv, double tono, double fm, double G, int p, int marcsize, trackDouble lpc)
    {
        trackDouble S, U;
        int period;
        double s;

        S = new double[marcsize];
        period = tono * fm;

        if(vNv)
            U = pulseGenerator(period, marcsize);
        else
            U = noiseGenerator(marcsize);

        for (int n = 0; n < marcsize; n++)
        {
            s = 0;
            for (int k = 1; k <= p; k++)
            {
                if(n-k < 0)
                    break;
                s += lpc[k-1]*S[n-k];
            }
            S[n] = s + G*U[n];
        }
        return S;
    }








    trackDouble Convolution(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel)
    {

        int j0, j1, k, nk, n;
        trackDouble convol = new double[lenTrack];
        float suma;

        nk = lenKernel;
        n = lenTrack;

        for (int i = 0; i < n; i++)
        {
            suma = 0;
            j0 = i;
            j1 = i + nk;
            for (int j = j0; j <= j1 && j < n; j++)
            {
                suma += Track[k] * kernel[nk + i - j];
            }
            convol[i] = suma;
        }

        return convol;
    }

    trackDouble ConvolutionCirc(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel)
    {

        int j0, j1, k, nk, n;
        trackDouble convol = new double[lenTrack];
        float suma;

        nk = lenKernel;
        n = lenTrack;

        for (int i = 0; i < n; i++)
        {
            suma = 0;
            j0 = i;
            j1 = i + nk;
            for (int j = j0; j <= j1; j++)
            {
                k = j + (j < 0 ? n : 0) - (j >= n ? n : 0);
                suma += Track[k] * kernel[nk + i - j];
            }
            convol[i] = suma;
        }

        return convol;
    }

    trackDouble Correlation(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel)
    {

        int j0, j1, k, nk, n;
        trackDouble correl = new double[lenTrack];
        float suma;

        nk = lenKernel;
        n = lenTrack;

        for (int i = 0; i < n; i++)
        {
            suma = 0;
            j0 = i;
            j1 = i + nk;
            for (int j = j0; j <= j1 && j < n; j++)
            {
                suma += Track[k] * kernel[j - i];
            }
            correl[i] = suma;
        }

        return correl;
    }

    trackDouble CorrelationCirc(trackDouble Track, int lenTrack, trackDouble kernel, int lenKernel)
    {
        int j0, j1, k, nk, n;
        trackDouble correl = new double[lenTrack];
        float suma;

        nk = lenKernel;
        n = lenTrack;

        for (int i = 0; i < n; i++)
        {
            suma = 0;
            j0 = i;
            j1 = i + nk;
            for (int j = j0; j <= j1; j++)
            {
                k = j + (j < 0 ? n : 0) - (j >= n ? n : 0);
                suma += Track[k] * kernel[nk + i - j];
            }
            correl[i] = suma;
        }

        return correl;
    }

    void Normal(int start, trackInt chunk, int lenChunk)
    {

        double max = -1;
        int end = start + lenChunk;
        for (int i = start; i < end; i++)
        {
            if (chunk[i] > max)
                max = abs(chunk[i]);
        }

        for (int i = start; i < end; i++)
        {
            chunk[i] /= max;
        }
    }

    void insert(int value, int pos, Histogram &H)
    {
        hist block;
        block.v = value;
        block.f = 1;
        H.insert(H.begin() + pos, block);
    }

    int BinarySearch(int value, Histogram &H)
    {
        int start = 0;
        int it = 0;
        int end = H.size() - 1;
        int middle = start + (end - start) / 2;
        double aux;

        if (0 == H.size())
        {
            insert(value, 0, H);
            return 0;
        }

        while (true)
        {
            if (H[middle].v == value)
            {
                H[middle].f++;
                break;
            }
            else if (value > H[middle].v)
            {
                if (middle == end)
                {
                    insert(value, middle + 1, H);
                    break;
                }
                start = middle + 1;
                aux = (double)(end - start) / 2;
                middle = start + round(aux);
            }
            else if (value < H[middle].v)
            {
                if (middle == start)
                {
                    insert(value, middle, H);
                    break;
                }
                end = middle - 1;
                aux = (end - start) / 2;
                middle = start + aux;
            }
            it++;
        }
        //cout << "iteraciones " << it << endl;
        return 0;
    }

    Histogram CreateHistogram(int start, trackInt &chunk, int length)
    {

        Histogram H;
        int end = start + length;

        for (int i = start; i < end; i++)
        {
            BinarySearch(chunk[i], H);
        }
        return H;
    }

    double EntropyTimeShort(int start, trackInt chunk, int length)
    {

        Histogram H;
        double sum = 0;
        double p, h;

        H = CreateHistogram(start, chunk, length);
        for (int i = 0; i < H.size(); i++)
        {
            p = H[i].f / (double)length;
            sum += p * log10(p);
        }
        h = (-1) * sum;
        return h;
    }

    int signo(int muestra)
    {
        return (muestra >= 0 ? 1 : -1);
    }

    int ZeroCrossing(int start, trackInt chunk, int lenChunk)
    {

        int cross = 0;
        int end = start + lenChunk;
        for (int i = start + 1; i < lenChunk; i++)
        {
            cross += abs(signo(chunk[i]) - signo(chunk[i - 1]));
        }

        return cross; ///(2*lenChunk);
    }

    int forwardSearch(double Emax, long lengthT, trackInt track, int framesize, int slide)
    {

        double E = 0;
        double Weighing = 0;
        double thresholdE = Emax;
        double thresholdC = 9e18;
        double NoiseE = 0;
        double SNR = 0;
        double e, c;
        int cross = 0;
        int count = 0;

        for (int pos = 0; pos < lengthT; pos += slide)
        {
            E = EnergyTimeShort(pos, track, framesize);
            cross = ZeroCrossing(pos, track, framesize);

            if (count < MTRASH)
            {
                if (NoiseE < E)
                    NoiseE = E;
                if (thresholdC > cross)
                    thresholdC = cross;
                count++;
            }
            else if (count == MTRASH)
            {
                SNR = 10 * log10(Emax / NoiseE);

                if (SNR < 20)
                {
                    NoiseE = Emax / pow(10, (SNR - 3) / 10);
                }
                else if (SNR < 30)
                {
                    NoiseE = Emax / pow(10, (SNR - 7) / 10);
                }
                else
                {
                    NoiseE = Emax / pow(10, 25 / 10);
                }
                //cout << "NoisEFor " << NoiseE << endl;
                count++;
            }
            else
            {
                e = E > NoiseE ? 0.6 : 0;
                c = cross < thresholdC && cross < 15 ? 0.4 : 0;
                Weighing = e + c;
            }
            if (Weighing >= 0.6)
                return pos;
        }

        return -1;
    }

    int backwardSearch(double Emax, long lengthT, trackInt track, int framesize, int slide)
    {

        double E = 0;
        double Weighing = 0;
        double thresholdE = Emax;
        double thresholdC = 9e18;
        double NoiseE = 0;
        double SNR = 0;
        int cross = 0;
        int count = 0;
        double e, c;

        trackInt marco = new int[framesize];

        for (int pos = lengthT - 1; pos > framesize; pos -= slide)
        {
            for (int i = 0; i < framesize; i++)
                marco[i] = track[pos - i];

            E = EnergyTimeShort(0, marco, framesize);
            cross = ZeroCrossing(0, marco, framesize);

            if (count < MTRASH)
            {
                if (NoiseE < E)
                    NoiseE = E;
                if (thresholdC > cross)
                    thresholdC = cross;
                count++;
            }
            else if (count == MTRASH)
            {
                SNR = 10 * log10(Emax / NoiseE);

                if (SNR < 20)
                {
                    NoiseE = Emax / pow(10, (SNR - 3) / 10);
                }
                else if (SNR < 30)
                {
                    NoiseE = Emax / pow(10, (SNR - 5) / 10);
                }
                else
                {
                    NoiseE = Emax / pow(10, 25 / 10);
                }
                //cout << "NoisEBac " << NoiseE << endl;
                //cout << "SNR " << SNR << endl;
                count++;
            }
            else
            {
                e = E > NoiseE ? 0.6 : 0;
                c = cross < thresholdC && cross < 15 ? 0.4 : 0;
                Weighing = e + c;
            }

            if (Weighing >= 0.6)
                return pos;
        }

        return -1;
    }

    int Segmenter(long lengthT, trackInt track, int framesize, int slide, int &start, int &end)
    {

        double E = 0;
        double NoisE = 0;
        double Emax = -1;

        for (int pos = 0; pos + framesize < lengthT - 1; pos += slide)
        {
            E = EnergyTimeShort(pos, track, framesize);
            if (E > Emax)
                Emax = E;
        }

        start = forwardSearch(Emax, lengthT, track, framesize, slide);
        end = backwardSearch(Emax, lengthT, track, framesize, slide);

        cout << "start: " << start << " end " << end << endl;
        if (-1 == start || -1 == end)
            return -1;

        return 1;
    }
} // namespace TimeTecnics

#endif