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
    trackDouble senal = new double[10];
    trackDouble low = new double[2];
    trackDouble high = new double[2];
    trackDouble lowI = new double[2];
    trackDouble highI = new double[2];
    matrizDouble prueba = new double*[2];
    index ind;
    int lengthInd;

    double c = sqrt(2);
    unsigned char pru = 0;
    unsigned char p = 128;
    double cielo;

    low[0] = 1.0/c; low[1] = 1.0/c;
    high[0] = 1.0 / c; high[1] = -1.0/c;


    lowI[0] = c/2; lowI[1] = c/2;
    highI[0] = c/2; highI[1] = -c/2;


    /*senal[0]= 3; senal[1]= 7; senal[2]= 6; senal[3]= 5; senal[4]= 7;
    senal[5]= 8; senal[6]= 10; senal[7]= 9; senal[8]= 9; senal[9]= 10;*/

    
    transDiscWav(senal, 10, high, low, 2, 3);

    trimTrack(senal, 10, 0.3, 3);

    prueba[0] = senal;

    ind = createIndex(prueba, 10, lengthInd, 1);

    cout << "Indice: ";
    for (int i = 0; i < lengthInd; i++)
    {
        cout << (int)ind[i] << " ";
    }cout << endl;

    // Guardar el archivo comprimido
    //saveWaveBin(audio, ind, lengthInd, senal, 10, "comprimido");
    

    // Leer el archivo guardado
    
    //readWaveBin(audio, "comprimido", senal, 10);
    //audio.printHeader();
    transDiscWavInv(senal, 10, highI, lowI, 2, 3);

    cout << "señal: ";
    for (int i = 0; i < 10; i++)
    {
        cout << senal[i] << " ";
    }cout << endl;

    return 0;
}