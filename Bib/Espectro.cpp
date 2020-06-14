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
    int filtros;
    int slice;
    string name;
    matrizDouble spect;

public:
    Spectrum(Wave &audio, int channel);
    Spectrum(string name);
    Spectrum();
    matrizDouble getSpect();
    int getMarcos();
    int getFrame();
    string getName();
    void clear();
    void save();
    void load(string name);
    void firstTrack(int i);
    void createPPM(string Nombre);
    void newSpectrum(Wave &audio, int channel);
    void createPPM();
    void divideTrack(trackInt data, long samples, int SamplesPerSec);
    double DTW(Spectrum &B);
    double DTWSandC(Spectrum &B);
    ~Spectrum();
};

string Spectrum::getName()
{
    return name;
}

void Spectrum::createPPM()
{
    string path = "Espect/" + name;
    createPPM(path);
}

Spectrum::Spectrum()
{
    spect = NULL;
    max = -1;
    min = 9e16;
    marcos = 0;
    filtros = 0;
}

Spectrum::Spectrum(string name)
{
    load(name);
}

void Spectrum::load(string Name)
{
    FILE *spectre;
    char cadena[100];
    char c;
    Name = PATHDICT + Name + ".sp";

    const char *punter = Name.c_str();

    spectre = fopen(punter, "r");
    if (spectre == NULL)
    {
        fprintf(stderr, "No pude abrir el archivo %s\n", punter);
        exit(0);
    }

    fscanf(spectre, "%s\n", cadena);
    name = cadena;
    fscanf(spectre, "%d,%d,\n", &bytepersample, &frameSize);
    fscanf(spectre, "%lf,%lf,%d,\n", &max, &min, &slice);
    fscanf(spectre, "%d,%d\n", &marcos,&filtros);

    spect = new double *[marcos];
    for (int i = 0; i < marcos; i++)
    {
        spect[i] = new double[filtros];
        for (int j = 0; j < filtros; j++)
        {
            fscanf(spectre, "%lf,", &spect[i][j]);
        }
        fscanf(spectre, "%c", &c);
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
    file << marcos << "," << filtros << "," << endl;

    for (int i = 0; i < marcos; i++)
    {
        for (int j = 0; j < filtros; j++)
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
    min = 9e16;
    marcos = 0;
    filtros = 0;
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
    printTrack(spect[i], filtros);
}

void Spectrum::newSpectrum(Wave &audio, int channel)
{
    clear();
    int factor = 128;
    bytepersample = audio.BytePorMu;
    frameSize = audio.getSamplesPerSec() * PERIOD;
    slice = audio.getSamplesPerSec() * SLICE;
    if (channel >= audio.wChannels)
    {
        cout << "Wrong in channel" << endl;
        exit(0);
    }
    while (frameSize > factor)
    {
        factor <<= 1;
    }
    frameSize = factor;
    divideTrack(audio.data[channel], audio.Samples, audio.getSamplesPerSec());
    name = audio.nameWave;
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
        exit(0);
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
    cout << "Eliminar: " << spect << endl;
    if (NULL != spect)
    {
        // cout << "Eliminar: "<<spect<<endl;
        for (int i = 0; i < marcos; i++)
        {
            delete[] spect[i];
        }
        delete[] spect;
    }
    //cout << "spectrum free" << endl;
}

void Spectrum::divideTrack(trackInt data, long Samples, int SamplesPerSec)
{
    int m, start, end;
    unsigned long int position, length;
    trackComplex marco;
    trackDouble filterBank, windows;
    double bandwith;

    Segmenter(Samples, data, frameSize, slice, start, end);

    length = end - start;
    max = -10;
    min = 9e16;
    marcos = (int)((length - frameSize) / slice);
    filtros = BANDS;
    spect = new trackDouble[marcos];
    windows = Hamming(frameSize);

    position = start;

    m = 0;
    //cout << "start: " << start << endl;
    //cout << "end: " << end << endl;
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
        filterBank = bandsBark(frameSize, marco, bandwith, max, min);
        
        delete[] marco;

        spect[m] = filterBank;
        position += slice;
        m++;
    }

    /*for (int i = 0; i < marcos; i++)
    {
        for (int j = 0; j < filtros; j++)
        {
            spect[i][j] /= max;
        }
        
    }*/
    
    //cout << "Max: " << max << endl;
    //cout << "Min: " << min << endl;
    delete[] windows;
}

double Spectrum::DTWSandC(Spectrum &B)
{

    int R = marcos, C = B.marcos, pr, pr1;
    double less, distI, a;
    double m = (double)(R - 1) / (double)(C - 1);
    double distance;
    matrizDouble costMatrix;
    int n = ceil(R / 4);

    /* Se crea la matriz de costos*/
    costMatrix = new trackDouble[R];
    for (int r = 0; r < R; r++)
    {
        //costMatrix[r] = (double *)calloc(C, sizeof(double));
        costMatrix[r] = new double[C];
    }

    /* Se llena la matriz de costos*/
    for (int c = 0; c < C; c++)
    {
        pr = m * (double)c;
        for (int r = 0; r < R; r++)
        {
            if (n >= abs(r - pr))
            {
                if (0 == c || 0 == r)
                    less = CosDistance(spect[r], B.spect[c], filtros);
                else
                {
                    less = 9e20;
                    pr1 = m * (double)(c - 1);
                    distI = CosDistance(spect[r], B.spect[c], filtros);
                    if (n >= abs((r - 1) - pr1))
                        less = costMatrix[r - 1][c - 1] + 2 * distI;

                    if (n >= abs(r - pr1))
                    {
                        a = costMatrix[r][c - 1] + distI;
                        less = less > a ? a : less;
                    }

                    if (n >= abs((r-1) - pr))
                    {
                        a = costMatrix[r - 1][c] + distI;
                        less = less > a ? a : less;
                    }
                }
                costMatrix[r][c] = less;
            }
        }
    }

    /*Se obtiene la distancia*/
    distance = costMatrix[R - 1][C - 1];

    /* Liberación de la memoria perteneciente a la matriz de costos*/
    for (int r = 0; r < R; r++)
    {
        delete[] costMatrix[r];
    }
    delete[] costMatrix;

    return distance;
}

double Spectrum::DTW(Spectrum &B)
{
    double less, n, m, distI, distII;
    matrizDouble costMatrix;
    int N = marcos, M = B.marcos;
    int r = 10;

    costMatrix = new trackDouble[N];
    for (int i = 0; i < N; i++)
    {
        costMatrix[i] = new double[M];
    }

    costMatrix[0][0] = CosDistance(spect[0], B.spect[0], filtros);
    for (int i = 1; i < M; i++)
    {
        if (i < r)
            costMatrix[0][i] = CosDistance(spect[0], B.spect[i], filtros) + costMatrix[0][i - 1];
        //else break;
    }
    for (int i = 1; i < N; i++)
    {
        //if(i<r)
        costMatrix[i][0] = CosDistance(spect[i], B.spect[0], filtros) + costMatrix[i - 1][0];
        //else break;
    }

    for (int i = 1; i < M; i++)
    {
        for (int j = 1; j < N; j++)
        {

            //if(abs(i-j)<r){
            distI = CosDistance(spect[j], B.spect[i], filtros);
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
    int limit_int = filtros;
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

            Acumulador = spect[j][filtros - i];
            Acumulador -= min;
            Acumulador = (Acumulador * 255.0) / aux;
            fprintf(ppm, "%d ", (int)Acumulador);
        }
        fprintf(ppm, "\n");
        // printf("\nfin\n");
    }

    //min=0;
    //max=255;

    fclose(ppm);
}

#endif