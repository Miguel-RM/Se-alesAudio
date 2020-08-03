#include <iostream>
#include "Bib/Wave.cpp"
#include "Bib/TimeTecnics.cpp"
#include "Bib/Miscellan.cpp"

using namespace ::std;
using namespace ::TimeTecnics;

int main()
{
    Wave audio("derecha_6");
    int p = 12;
    double nm;
    int marcsize = 256;
    int lengthT = audio.getSamples();
    int fm = audio.getSamplesPerSec(), IPOS;
    trackInt channel0;
    trackDouble y = new double[lengthT];
    trackDouble track, autocorr, R, lpc;
    trackDouble pulsos;
    trackDouble synthetic;

    bool vNv;
    double tono, G, IPK;

    cout << "Numero de canales: " << audio.getSamplesPerSec() << endl;

    channel0 = audio.getTrack(0);
    track = preEnfasis(channel0, lengthT);
    synthetic = new double[audio.getSamples()];
    nm = (double)audio.getSamples();
    nm /= (double)marcsize;

    cout << "Numero de marcos: "<< nm << endl;

    autocorr = autoCorrMod(2, marcsize, track);
    searchPeak(autocorr, marcsize, IPOS, IPK);
    cout << "Pico: "<< IPOS << endl;

    for (int i = 0; i < 3; i++)
    {
        vocNovoc(vNv, tono, G, i, marcsize, track, R, fm, p);
        lpc = cLPC(p, R);
        
        pulsos = voiceSynthesizer(vNv, tono, fm, G, p, marcsize, lpc);
        for (int j = 0; j < marcsize; j++)
        {
            synthetic[i*marcsize+j] = pulsos[j];
        }
        
        //cout << "Vocalizado: "; if (vNv) cout << "True" << endl; else cout << "False" << endl;
        //cout << "Tono: " << tono * fm<< endl;
        //cout << "G: " << G << endl;
    }

    audio.newTrack(audio.getSamples(), synthetic, 0);
    audio.writeWave("S");


    /*cout << "lpc = [ ";
    for (int i = 0; i < p; i++)
    {
        cout << lpc[i] << " ";
        //cout << autocorr[i] << " ";
    }cout << " ]; "<< endl;*/

    int w =0;
    
    cout << "a = [ ";
    for (int i = 256*w; i < 256*w+256; i++)
    {
        cout << pulsos[i] << " ";
        //cout << track[i] << " ";
        //cout << autocorr[i] << " ";
    }cout << " ]; "<< endl;

    return 0;
}
