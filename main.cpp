#include "Wave.cpp"
#include "Espectro.cpp"
#include "Fourier.cpp"
#include "Filtros.cpp"
#include "tracks.cpp"

using namespace::std;

int main()
{
    Wave audio("audio");
    Spectrum marcos(audio);
    marcos.firstTrack(2);
    audio.newTrack(marcos.getSpectR(),marcos.getSpectI());
    audio.printTrack();
    audio.writeWave();
    cout << "Tamaño: " << sizeof(unsigned char) << endl;
    //audio.printTrack();
    return 0;
}