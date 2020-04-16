#include "Wave.cpp"
#include "Fourier.cpp"
#include "tracks.cpp"

#ifndef SPECTRUM_CPP
#define SPECTRUM_CPP


using namespace::Fourier;
using namespace::Tracks;

class Spectrum
{

friend class Wave;
private:
    /* data */
    int frameSize;
    int marcos;
    short bytepersample;
    matrizComplex spect;
    
public:
    Spectrum(Wave &audio, int channel);
    matrizComplex getSpect();
    int getMarcos();
    int getFrame();
    void firstTrack(int i);
    void divideTrack(int mode, trackInt data, long samples);
    Wave generateWave();
    ~Spectrum();
};

int Spectrum::getFrame()
{
    return frameSize;
}

int Spectrum::getMarcos()
{
    return marcos;
}

matrizComplex Spectrum::getSpect()
{
    return spect;
}

void Spectrum::firstTrack(int i)
{
    printTrackC(spect[i], frameSize);
}

Spectrum::Spectrum(Wave &audio, int channel)
{
    int factor = 128;
    bytepersample = audio.BytePorMu;
    frameSize = audio.getSamplesPerSec() * Period;
    while (frameSize > factor)
    {
        factor <<=1;
    }
    frameSize = factor; 
    divideTrack(0, audio.data[channel], audio.Samples);
    cout << "Marcos: " << marcos;
    cout << " muestras: " << audio.Samples;
    cout << " framesize: "<< frameSize << endl;
    
}

Spectrum::~Spectrum()
{
}

void Spectrum::divideTrack(int mode, trackInt data, long Samples)
{
    int splice, m;
    unsigned long int position, length;
    trackComplex track;

    if(0 == mode) splice = frameSize; else splice = 86;
    marcos = (int)round(Samples/frameSize);
    spect = new trackComplex[marcos];
    
    
    position = 0;
    length = Samples;

    m=0;
    cout<<"Data : "<<data[0]<<endl;
    while (position < length)
    {
        

        track = new complex<double>[frameSize];

        for (unsigned long i = position, j=0; i < (position+frameSize); i++, j++)
        {
            if(i<length)
                track[j]=(double)data[i];
        }
        if(position==0) cout << "Legueaqui: " <<track[0]<<endl;
        FFT(track, frameSize, -1);
        spect[m] = track;
        
        position += frameSize;
        m++;
        
    }
}

#endif

