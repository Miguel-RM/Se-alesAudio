#include "Wave.cpp"
#include "Fourier.cpp"
#include "tracks.cpp"

#ifndef SPECTRUM_CPP
#define SPECTRUM_CPP
#define Period 30e-3

using namespace::Fourier;
using namespace::Tracks;

class Spectrum
{

friend class Wave;
private:
    /* data */
    int frameSize;
    Matrix spectR;
    Matrix spectI;
    
public:
    Spectrum(Wave &audio);
    Matrix& getSpectR();
    Matrix& getSpectI();
    void firstTrack(int i);
    void divideTrack(int mode, track &data);
    Wave generateWave();
    ~Spectrum();
};

Matrix& Spectrum::getSpectR()
{
    return spectR;
}

Matrix& Spectrum::getSpectI()
{
    return spectI;
}

void Spectrum::firstTrack(int i)
{
    printTrack(spectR[i]);
    printTrack(spectI[i]);
}

Spectrum::Spectrum(Wave &audio)
{
    int factor = 128;
    frameSize = audio.getSamplesPerSec() * Period;
    while (frameSize > factor)
    {
        factor <<=1;
    }
    frameSize = factor; 
    divideTrack(0, audio.data);
    cout << "Marcos: " << spectR.size();
    cout << " muestras: " << audio.data.size();
    cout << " framesize: "<< spectR[0].size() << endl;
    
}

Spectrum::~Spectrum()
{
}

void Spectrum::divideTrack(int mode, track &data)
{
    int splice;
    unsigned long int position, length;
    trackDou real, imag, salidaR, salidaI;
    if(0 == mode) splice = frameSize; else splice = 86;

    position = 0;
    length = data.size();

    while (position < length)
    {
        imag = generaCeros(frameSize);
        salidaR = salidaI = imag;
        real.clear();
        
        for (unsigned long int i = position; i < (position+frameSize); i++)
        {
            if(i<length)
                real.push_back((double)data[i]);
            else 
                real.push_back(0.0);
        }
        tRF1D(real, imag, salidaR, salidaI, -1);
        spectR.push_back(salidaR);
        spectI.push_back(salidaI);
        
        position += frameSize;
        
    }
    
}

#endif

