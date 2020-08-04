#include <iostream>
#include "Bib/Wave.cpp"
#include "Bib/TimeTecnics.cpp"
#include "Bib/Miscellan.cpp"
#include "Bib/Wavelets.cpp"

using namespace ::std;
using namespace ::TimeTecnics;
using namespace ::Wavelet;

int main()
{
    Wave audio("derecha_6");
    trackDouble senal;
    trackDouble low = new double[2];
    trackDouble high = new double[2];
    trackDouble lowI = new double[2];
    trackDouble highI = new double[2];
    matrizDouble prueba = new double*[2];
    index ind;
    int lengthInd;
    int lengthT;
    int nivel = 1;

    double c = sqrt(2);

/**********************************************************************
 *   En esta sección se realiza la creación de los filtros            *
 * *******************************************************************/

    low[0] = 1.0/c; low[1] = 1.0/c;
    high[0] = 1.0 / c; high[1] = -1.0/c;


    lowI[0] = c/2; lowI[1] = c/2;
    highI[0] = c/2; highI[1] = -c/2;

/**********************************************************************
 *   En esta sección se realiza la compresión del audio               *
 * *******************************************************************/

/*
    senal = audio.getTrackD(0);
    lengthT = audio.getSamples();
    cout << "Muestras: " << lengthT << endl;
    transDiscWav(senal, lengthT, high, low, 2, nivel);

    trimTrack(senal, lengthT, 0.0, nivel);

    prueba[0] = senal;

    ind = createIndex(prueba, lengthT, lengthInd, 1);

    cout << "Indice: ";
    for (int i = 0; i < lengthInd; i++)
    {
        cout << (int)ind[i] << " ";
    }cout << endl;

    audio.newTrack(lengthT, senal, 0);
    // Guardar el archivo comprimido
    saveWaveBin(audio, ind, lengthInd, "comprimido2");
    
    delete[] senal;
//*/

/**********************************************************************
 *   En esta sección se realiza la descompresión del audio            *
 * *******************************************************************/

    // Leer el archivo guardado

    readWaveBin(audio, "comprimido2");
    senal = audio.getTrackD(0);
    lengthT = audio.getSamples();
    audio.printHeader();
    transDiscWavInv(senal, lengthT, highI, lowI, 2, nivel);
    audio.newTrack(lengthT, senal, 0);

    audio.writeWave("Descom");
//*/

    return 0;
}