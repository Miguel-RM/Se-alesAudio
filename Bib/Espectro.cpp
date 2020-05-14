#include "Wave.cpp"
#include "Fourier.cpp"
#include "tracks.cpp"
#include "TimeTecnics.cpp"
#include "FreqTecnics.cpp"
#include <fstream>

#ifndef SPECTRUM_CPP
#define SPECTRUM_CPP

using namespace ::Fourier;
using namespace ::Tracks;
using namespace ::TimeTecnics;
using namespace ::FreqTecnics;

class Spectrum
{

    friend class Dictionary;

private:
    /* data */
    int bytepersample;
    int frameSize;
    double max;
    double min;
    int marcos;
    int slice;
    string name;
    matrizDouble spect;

public:
    Spectrum(Wave &audio, int channel);
    Spectrum();
    matrizDouble getSpect();
    int getMarcos();
    int getFrame();
    void clear();
    void save();
    void load(string name);
    void firstTrack(int i);
    void createPPM(string Nombre);
    void divideTrack(trackInt data, long samples, int SamplesPerSec);
    double DTW(Spectrum &B);
    ~Spectrum();
};

Spectrum::Spectrum()
{
    spect = NULL;
    max = -1;
    min = 9999999;
    marcos = 0;
}

void Spectrum::load(string Name)
{
    FILE *spectre;
    char cadena[100];
    char c;
    int aux;
    Name = PATHDICT + Name + ".sp";

    const char *punter = Name.c_str();

    spectre = fopen(punter, "r");
    if (spectre == NULL)
    {
        fprintf(stderr, "No pude abrir el archivo %s\n", punter);
        exit(0);
    }

    fscanf(spectre, "%s\n", cadena);
    name=cadena;
    fscanf(spectre, "%d,%d,\n", &bytepersample, &frameSize);
    fscanf(spectre, "%lf,%lf,%d,\n", &max,&min,&slice);
    fscanf(spectre, "%d,", &marcos);
    fscanf(spectre, "%d,\n", &aux);

    spect = new double*[marcos];
    for (int i = 0; i < marcos; i++)
    {
        spect[i] = new double[BANDS];
        for (int j = 0; j < BANDS; j++)
        {
            fscanf(spectre, "%lf,", &spect[i][j]);
        }fscanf(spectre, "%c",&c);
    }
    
    fclose(spectre);
}

void Spectrum::save()
{
    string path = "dictionary/" + name + ".sp";
    cout << path << endl;
    ofstream file(path);
    if (file.bad())
    {
        cout << "Wrong to config File" << endl;
        exit(0);
    }
    file << name << endl;
    file << bytepersample << ",";
    file << frameSize << "," << endl;
    file << max << ",";
    file << min << ",";
    file << slice << "," << endl;
    file << marcos << "," << BANDS << "," << endl;

    for (int i = 0; i < marcos; i++)
    {
        for (int j = 0; j < BANDS; j++)
        {
            file << spect[i][j] << ",";
        }
        file << endl;
    }

    file.close();
}

void Spectrum::clear()
{
    spect = NULL;
    max = -1;
    min = 9999999;
    marcos = 0;
}

int Spectrum::getFrame()
{
    return frameSize;
}

int Spectrum::getMarcos()
{
    return marcos;
}

matrizDouble Spectrum::getSpect()
{
    return spect;
}

void Spectrum::firstTrack(int i)
{
    printTrack(spect[i], BANDS);
}

Spectrum::Spectrum(Wave &audio, int channel)
{
    int factor = 128;
    bytepersample = audio.BytePorMu;
    frameSize = audio.getSamplesPerSec() * PERIOD;
    slice = audio.getSamplesPerSec() * SLICE;
    if (channel >= audio.wChannels)
    {
        cout << "Wrong in channel" << endl;
        exit(0160);
    }
    while (frameSize > factor)
    {
        factor <<= 1;
    }
    frameSize = factor;
    divideTrack(audio.data[channel], audio.Samples, audio.getSamplesPerSec());
    name = audio.nameWave;
}

Spectrum::~Spectrum()
{

    for (int i = 0; i < marcos; i++)
    {
        delete[] spect[i];
    }
    delete[] spect;
    //cout << "spectrum free" << endl;
}

void Spectrum::divideTrack(trackInt data, long Samples, int SamplesPerSec)
{
    int m, start, end, banda;
    unsigned long int position, length;
    trackComplex marco;
    trackDouble bark, windows;
    double bandwith, magnitude, energy;

    Segmenter(Samples, data, frameSize, slice, start, end);

    length = end - start;
    max = -10;
    min = 999999;
    marcos = (int)((length - frameSize) / slice);
    spect = new trackDouble[marcos];
    windows = Hamming(frameSize);

    position = start;

    m = 0;
    cout << "start: " << start << endl;
    cout << "end: " << end << endl;
    while (m < marcos)
    {

        marco = new complex<double>[frameSize];
        for (unsigned long i = position, j = 0; i < (position + frameSize); i++, j++)
        {
            if (i < end)
                marco[j] = (double)data[i] * windows[j];
        }
        // Se obtiene el espectro de frecuencias a partir del marco

        TRFW(marco, frameSize, FFTW_FORWARD);

        // se obtiene la energia en las banadas de bark
        bandwith = SamplesPerSec / frameSize;
        bark = (double *)calloc(BANDS, sizeof(double));
        for (int i = 0; i < frameSize / 2; i++)
        {
            magnitude = abs(marco[i]);
            energy = magnitude * magnitude;
            if (energy > max)
                max = energy;
            if (energy < min)
                min = energy;
            banda = BarkScale(i * bandwith);
            if (banda < BANDS)
                bark[banda] += energy;
            else
                break;
        }

        delete[] marco;
        spect[m] = bark;

        position += slice;
        m++;
    }
    //cout << "Max: "<< max <<endl;
    //cout << "Min: "<< min <<endl;
    delete[] windows;
}

double Spectrum::DTW(Spectrum &B)
{
    double less, n, m, distI, distII;
    matrizDouble costMatrix;
    int N = marcos, M = B.marcos;
    int r;

    costMatrix = new trackDouble[N];
    for (int i = 0; i < N; i++)
    {
        costMatrix[i] = new double[M];
    }

    costMatrix[0][0] = CosDistance(spect[0], B.spect[0], BANDS);
    for (int i = 1; i < M; i++)
    {
        //if(i<r)
        costMatrix[0][i] = CosDistance(spect[0], B.spect[i], BANDS) + costMatrix[0][i - 1];
        //else break;
    }
    for (int i = 1; i < N; i++)
    {
        //if(i<r)
        costMatrix[i][0] = CosDistance(spect[i], B.spect[0], BANDS) + costMatrix[i - 1][0];
        //else break;
    }

    for (int i = 1; i < M; i++)
    {
        for (int j = 1; j < N; j++)
        {

            //if(abs(i-j)<r){
            distI = CosDistance(spect[j], B.spect[i], BANDS);
            less = costMatrix[j - 1][i - 1] + 2 * distI;
            distII = costMatrix[j][i - 1] + distI;

            if (less > distII)
            {
                less = distII;
            }
            distII = costMatrix[j - 1][i] + distI;

            if (less > distII)
            {
                less = distII;
            }

            costMatrix[j][i] = less;
            //}
        }
    }

    distI = costMatrix[N - 1][M - 1] / (N + M);

    for (int i = 0; i < N; i++)
    {
        delete[] costMatrix[i];
    }
    delete[] costMatrix;

    return distI;
}

void Spectrum::createPPM(string Nombre)
{
    FILE *ppm;
    Nombre = Nombre + ".ppm";
    const char *Nom = Nombre.c_str();
    ppm = fopen(Nom, "w");
    int limit_ext = marcos;
    int limit_int = BANDS;
    double Acumulador, aux = max - min;

    if (ppm == NULL)
    {
        fprintf(stderr, "No pude crear la imagen %s\n", Nom);
        exit(0);
    }

    fprintf(ppm, "P2\n");
    fprintf(ppm, "#Espectrograma\n");
    fprintf(ppm, "%d %d\n", limit_ext, limit_int);
    fprintf(ppm, "255\n");

    //printf("mminimo %f Maximno %f\n",min,max);

    for (int i = 0; i < limit_int; i++)
    {
        for (int j = 0; j < limit_ext; j++)
        {

            Acumulador = spect[j][16 - i];
            Acumulador -= min;
            Acumulador = (Acumulador * 255) / aux;
            //mEG[j][17-i]=Acumulador;
            fprintf(ppm, "%d ", (int)Acumulador);
            //printf("banda %d; %f \n",j,Acumulador);
        }
        fprintf(ppm, "\n");
        // printf("\nfin\n");
    }

    //min=0;
    //max=255;

    fclose(ppm);
}

#endif
