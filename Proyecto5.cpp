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
    int marcsize = 256, avan = 80;
    int lengthT = audio.getSamples();
    int fm = audio.getSamplesPerSec(), IPOS;
    trackInt channel0;
    trackDouble track, R, lpc;
    trackDouble pulsos;
    trackDouble synthetic;
    trackDouble hamm;

    bool vNv;
    double tono, G, IPK;

    cout << "Numero de canales: " << audio.getSamplesPerSec() << endl;

    channel0 = audio.getTrack(0);
    track = preEnfasis(channel0, lengthT);

    synthetic = (double *)calloc(audio.getSamples(), sizeof(double));
    nm = (double)audio.getSamples();
    nm /= (double)avan;

    cout << "Numero de marcos: "<< nm << endl;

    hamm = Hamming(marcsize);

    for (int i = 0; i < nm; i++)
    {
        vocNovoc(vNv, tono, G, i, marcsize, track, R, hamm, fm, p);
        lpc = cLPC(p, R);
        
        pulsos = voiceSynthesizer(vNv, tono, fm, G, p, avan, lpc);
        for (int j = 0; j < avan; j++)
        {
            synthetic[i*avan+j] = pulsos[j];
        }
        delete[] lpc;
        delete[] R;
        //cout << "Vocalizado: "; if (vNv) cout << "True" << endl; else cout << "False" << endl;
        //cout << "Tono: " << tono * fm<< endl;
        //cout << "G: " << G << endl;
    }

    audio.newTrack(audio.getSamples(), synthetic, 0);
    audio.writeWave("S");

    return 0;
}
