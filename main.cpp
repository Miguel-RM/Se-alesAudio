#include "Wave.cpp"
#include "Espectro.cpp"
#include "Fourier.cpp"
#include "Filtros.cpp"
#include "tracks.cpp"

using namespace::std;
using namespace::Filters;

int main()
{
    Wave audio("meg1");
    int desp = 510;
    Spectrum marcos1(audio,0);
    Spectrum marcos2(audio,1);
    trackFloat gauss;

    gauss = gaussiana(10,0,0);
    
    aplyKernel(gauss, marcos1.getSpectR(),desp);
    aplyKernel(gauss, marcos1.getSpectI(),desp);
    aplyKernel(gauss, marcos2.getSpectR(),desp);
    aplyKernel(gauss, marcos2.getSpectI(),desp);
    //marcos.firstTrack(0);
    

    audio.newTrack(marcos1.getSpectR(),marcos1.getSpectI(), 0);
    audio.newTrack(marcos2.getSpectR(),marcos2.getSpectI(), 1);
    audio.writeWave("especial");
    //audio.printTrack(0);
    
    cout << "Tamaño: " << sizeof(unsigned char) << endl;
    //audio.printTrack();
    return 0;
}