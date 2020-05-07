#include "Bib/Wave.cpp"
#include "Bib/Fourier.cpp"
#include "Bib/Filtros.cpp"
#include "Bib/tracks.cpp"

using namespace ::std;
using namespace ::Filters;

int main()
{
    Wave audio("AudiosIn/oboe-bassoon"); 

    int desp = 0, sigma = 3000; // datos necesarios para la gaussiana
    int lengthT = audio.getSamples();
    int lengthK = 6 * sigma + 1;
    trackDouble gauss;

    trackComplex chan1, chan2;

    // Se crea la gaussiana y se imprime en la terminal
    gauss = gaussiana(sigma, 0);
    //printTrack(gauss, lengthK);

    // los canales del audio son del tipo entero por lo que es necesario
    // tranformarlos a complex

    chan1 = audio.int2Complex(0);
    chan2 = audio.int2Complex(1);
    lengthT = audio.getLenComplex();
    //De esta manera es posible aplicar la transformada rapida de fourier

    TRFW(chan1, lengthT, FFTW_FORWARD);
    TRFW(chan2, lengthT, FFTW_FORWARD);

    //la siguiente funcion se encarga de realizar la multiplicacion del
    // espectro de frecuencias con la gaussiana creada anteriormente
    SpectrepuntKern(lengthT, lengthK, chan1, gauss, desp);
    SpectrepuntKern(lengthT, lengthK, chan2, gauss, desp);

    //Ahora se devuelve al dominio del tiempo
    TRFW(chan1, lengthT, FFTW_BACKWARD);
    TRFW(chan2, lengthT, FFTW_BACKWARD);

    //Una vez en el dominio del tiempo se guardan las pistas en el audio
    audio.newTrack(lengthT, chan1, 0);
    audio.newTrack(lengthT, chan2, 1);

    //para finalizar se escribe el audio en un archivo wave.
    audio.writeWave("baja");

    return 0;
}