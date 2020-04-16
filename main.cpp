#include "Bib/Wave.cpp"
#include "Bib/Espectro.cpp"
#include "Bib/Fourier.cpp"
#include "Bib/Filtros.cpp"
#include "Bib/tracks.cpp"

using namespace::std;
using namespace::Filters;

int main()
{
    Wave audio("AudiosIn/oboe-bassoon"); //bosson desp= 5 sigma=3
    //audio.printTrack(0);
    int desp = 15, sigma = 3;
    Spectrum marcos1(audio,0);
    Spectrum marcos2(audio,1);
    trackDouble gauss;

    gauss = gaussiana(sigma,0,0);
    printTrack(gauss, 6*sigma+1);
    
    aplyKernel(marcos1.getMarcos(), marcos1.getFrame(), sigma, gauss, marcos1.getSpect(),desp);
    aplyKernel(marcos2.getMarcos(), marcos2.getFrame(), sigma, gauss, marcos2.getSpect(),desp);
    //marcos1.firstTrack(0);
    

    audio.newTrack(marcos1.getMarcos(), marcos1.getFrame(), marcos1.getSpect(), 0);
    audio.newTrack(marcos2.getMarcos(), marcos2.getFrame(), marcos2.getSpect(), 1);
    audio.writeWave("baja");
    //audio.printTrack(0);
    
    cout << "Tamaño: " << sizeof(unsigned char) << endl;
    //audio.printTrack();*/
    
   
    return 0;
}