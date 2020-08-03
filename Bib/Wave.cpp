#include <iostream>
#include <stdlib.h>
#include "estructuras.cpp"
#include "Fourier.cpp"
#include "tracks.cpp"
//#include"Miscellan.cpp"

using namespace ::std;
using namespace ::audio;
using namespace ::Fourier;
using namespace ::Tracks;
//using namespace::Miscellan;

#ifndef WAVE_CPP
#define WAVE_CPP

class Wave
{

    friend class Spectrum;

private:
    /* data */
    unsigned char groupID[5];  /* dede ser 'RIFF' */
    unsigned int fileSize;     /* sin contar los ultimos 15 bytes*/
    unsigned char riffType[5]; /* debe ser 'WAVE' */
    unsigned char chunkID[5];  /* debe ser 'fmt ' */
    unsigned int chunkSize;    /* tamano del EWavezado que falta por leer*/
    unsigned short wFormatTag; /* debe ser 1 (sin compresion) */
    unsigned short wChannels;
    unsigned int dwSamplesPerSec;  /*regimen de muestreo */
    unsigned int dwAvgBytesPerSec; /* Para que el player sepa tam de buffer*/
    unsigned short wBlockAlign;    /* wChannels)*(wBitsPerSample/8) */
    unsigned short wBitsPerSample;
    unsigned char dataID[5]; /* debe ser 'data' */
    unsigned int signalSize; /* tamano de la senal en muestras */
    unsigned int BytePorMu;
    unsigned int Samples;
    //Matrix32 data; // el audio
    matrizInt data;
    double averageL; // La energia promedio del audio
    double averageR;
    long lenComplex;
    string nameWave; //path y nombre del audio
    bool Full;
    void readWave();

public:
    void newTrack(int marcos, int frameSize, matrizComplex spectre, int channel);
    friend void saveWaveBin(Wave &audio, index ind, int lengthInx, string name);
    void newTrack(int length, trackDouble chanTrack, int channel);
    void newTrack(int length, trackComplex spectre, int channel);
    friend void readWaveBin(Wave &audio, string name);
    trackComplex int2Complex(int channel);
    trackInt getTrack(int channel);
    void printTrack(int channel);
    void loadWave(string Name);
    void writeWave(string add);
    int getSamplesPerSec();
    int getLenComplex();
    void printHeader();
    int getChannels();
    Wave(string Name);
    int getSamples();
    void clear();
    ~Wave();
    Wave();
};

void saveWaveBin(Wave &audio, index ind, int lengthInx, string name)
{

    FILE *pFile;
    int Samples, limit;
    name += ".wvlt";
    char aux;
    short auxi, auxr, chann;
    const char *punter = name.c_str();

    // Manejo del indice
    unsigned char p = 128;
    short k = 1;
    int l = 0;

    pFile = fopen(punter, "w");

    if (pFile == NULL)
    {
        fprintf(stderr, "No pude escribir el archivo %s\n", punter);
        exit(0);
    }

    Samples = audio.signalSize / audio.BytePorMu;

    // se amacena la cabecera del archivo wav
    fwrite(&audio.groupID, 4, 1, pFile);
    fwrite(&audio.fileSize, 4, 1, pFile);
    fwrite(&audio.riffType, 4, 1, pFile);
    fwrite(&audio.chunkID, 4, 1, pFile);
    fwrite(&audio.chunkSize, 4, 1, pFile);
    fwrite(&audio.wFormatTag, 2, 1, pFile);
    fwrite(&audio.wChannels, 2, 1, pFile);
    fwrite(&audio.dwSamplesPerSec, 4, 1, pFile);
    fwrite(&audio.dwAvgBytesPerSec, 4, 1, pFile);
    fwrite(&audio.wBlockAlign, 2, 1, pFile);
    fwrite(&audio.wBitsPerSample, 2, 1, pFile);
    fwrite(&audio.dataID, 4, 1, pFile);
    fwrite(&audio.signalSize, 4, 1, pFile);

    // se almacena la longitud y el indice
    fwrite(&lengthInx, 4, 1, pFile);

    for (int i = 0; i < lengthInx; i++)
    {
        fwrite(&ind[i], 1, 1, pFile);
    }

    cout << "length: " << Samples * audio.BytePorMu << endl;

    chann = 0;
    limit = Samples * audio.wChannels;
    
    for (int i = 0, j = 0; j < limit; i++, j++, k++)
    {
        if(i == Samples)
        {
            chann = 1;
            i = 0;
        }

        if ((ind[l] & p) != 0)
        {
            auxi = (short)audio.data[chann][i];
            fwrite(&auxi, audio.BytePorMu, 1, pFile);
        }
        if (8 == k)
        {
            k = 1;
            p = 128;
            l++;
        }
    }

    fclose(pFile);
}

void readWaveBin(Wave &audio, string Name)
{

    int i, limit;
    FILE *pFile;
    audio.nameWave = Name;
    Name = Name;
    Name += ".wvlt";
    cout << Name << endl;
    const char *punter = Name.c_str();
    trackInt track;
    int sampleLeft = 0;

    // variables del manejo de indice
    int lengthInd;
    index ind;
    unsigned char p = 128;
    short k = 1;
    int l = 0;

    pFile = fopen(punter, "r");

    if (pFile == NULL)
    {
        fprintf(stderr, "No pude abrir el archivo %s\n", punter);
        exit(0);
    }

    audio.clear();


    // Lectura de la cabecera
    fread(&audio.groupID, 4, 1, pFile);
    audio.groupID[4] = 0;
    fread(&audio.fileSize, 4, 1, pFile);
    fread(&audio.riffType, 4, 1, pFile);
    audio.riffType[4] = 0;
    fread(&audio.chunkID, 4, 1, pFile);
    fread(&audio.chunkSize, 4, 1, pFile);
    fread(&audio.wFormatTag, 2, 1, pFile);
    fread(&audio.wChannels, 2, 1, pFile);
    fread(&audio.dwSamplesPerSec, 4, 1, pFile);
    fread(&audio.dwAvgBytesPerSec, 4, 1, pFile);
    fread(&audio.wBlockAlign, 2, 1, pFile);
    fread(&audio.wBitsPerSample, 2, 1, pFile);
    fread(&audio.dataID, 4, 1, pFile);
    audio.dataID[4] = 0;
    fread(&audio.signalSize, 4, 1, pFile);
    audio.BytePorMu = audio.wBitsPerSample / 8;
    audio.Samples = audio.signalSize / audio.BytePorMu;

    //Lectura de el indice
    fread(&lengthInd, 4, 1, pFile);
    ind = new unsigned char[lengthInd];

    for (int i = 0; i < lengthInd; i++)
    {
        fread(&ind[i], 1, 1, pFile);
    }
    
    // Lectura de las muestras de audio
    i = 0;
    audio.data = new int *[audio.wChannels];
    track = new int[audio.Samples];

    
    limit = audio.wChannels * audio.Samples;
    for (int i = 0, j=0; i < limit; i++, j++, k++)
    {
        if(audio.Samples == j)
        {
            j=0;
            audio.data[0] = track;
            track = new int[audio.Samples];

        }
        if((ind[l] & p) != 0)
        {
            fread(&sampleLeft, audio.BytePorMu, 1, pFile);
            track[i] = (short)sampleLeft;
        }

        if (8 == k)
        {
            k = 1;
            p = 128;
            l++;
        }
    }
    if(2 == audio.wChannels)
        audio.data[1] = track;
    else 
        audio.data[0] = track;

    delete[] ind;

    audio.Full = true;
}

int Wave::getChannels()
{
    return wChannels;
}

int Wave::getLenComplex()
{
    return lenComplex;
}

trackInt Wave::getTrack(int channel)
{
    return data[channel];
}

int Wave::getSamples()
{
    return Samples;
}

int Wave::getSamplesPerSec()
{
    return dwSamplesPerSec;
}

Wave::Wave()
{
    Full = false;
}

void Wave::clear()
{
    if (Full)
    {
        Full = false;
        delete[] data[0];
        if (2 == wChannels)
            delete[] data[1];
        delete[] data;
    }
}

void Wave::loadWave(string Name)
{
    int i;
    FILE *pFile;
    nameWave = Name;
    Name = PATH + Name;
    Name += ".wav";
    cout << Name << endl;
    const char *punter = Name.c_str();

    pFile = fopen(punter, "r");

    if (pFile == NULL)
    {
        fprintf(stderr, "No pude abrir el archivo %s\n", punter);
        exit(0);
    }

    clear();

    fread(&groupID, 4, 1, pFile);
    groupID[4] = 0;
    fread(&fileSize, 4, 1, pFile);
    fread(&riffType, 4, 1, pFile);
    riffType[4] = 0;
    fread(&chunkID, 4, 1, pFile);
    fread(&chunkSize, 4, 1, pFile);
    fread(&wFormatTag, 2, 1, pFile);
    fread(&wChannels, 2, 1, pFile);
    fread(&dwSamplesPerSec, 4, 1, pFile);
    fread(&dwAvgBytesPerSec, 4, 1, pFile);
    fread(&wBlockAlign, 2, 1, pFile);
    fread(&wBitsPerSample, 2, 1, pFile);
    fread(&dataID, 4, 1, pFile);
    dataID[4] = 0;
    fread(&signalSize, 4, 1, pFile);

    BytePorMu = wBitsPerSample / 8;
    Samples = signalSize / BytePorMu;
    fclose(pFile);
    readWave();
    Full = true;
}

void Wave::printHeader()
{
    printf("groupID=%s\n",groupID);
    printf("fileSize=%u\n",fileSize);
    printf("riffType=%s\n",riffType);
    printf("chunkID=%s\n",chunkID);
    printf("wformatTag=%u\n",wFormatTag);
    printf("wChannels=%u\n",wChannels);
    printf("dwSamplesPerSec=%u\n",dwSamplesPerSec);
    printf("dwAvgBytesPerSec=%u\n",dwAvgBytesPerSec);
    printf("wBlockAlign=%u\n",wBlockAlign);
    printf("wBitsPerSample=%u\n",wBitsPerSample);
    printf("dataID=%s\n",dataID);
    printf("chunkSize=%u\n",signalSize);
}

Wave::Wave(string Name)
{
    int i;
    FILE *pFile;
    nameWave = Name;
    Name = PATH + Name;
    Name += ".wav";
    cout << Name << endl;
    const char *punter = Name.c_str();

    pFile = fopen(punter, "r");

    if (pFile == NULL)
    {
        fprintf(stderr, "No pude abrir el archivo %s\n", punter);
        exit(0);
    }

    fread(&groupID, 4, 1, pFile);
    groupID[4] = 0;
    fread(&fileSize, 4, 1, pFile);
    fread(&riffType, 4, 1, pFile);
    riffType[4] = 0;
    fread(&chunkID, 4, 1, pFile);
    fread(&chunkSize, 4, 1, pFile);
    fread(&wFormatTag, 2, 1, pFile);
    fread(&wChannels, 2, 1, pFile);
    fread(&dwSamplesPerSec, 4, 1, pFile);
    fread(&dwAvgBytesPerSec, 4, 1, pFile);
    fread(&wBlockAlign, 2, 1, pFile);
    fread(&wBitsPerSample, 2, 1, pFile);
    fread(&dataID, 4, 1, pFile);
    dataID[4] = 0;
    fread(&signalSize, 4, 1, pFile);

    BytePorMu = wBitsPerSample / 8;
    Samples = signalSize / BytePorMu;
    fclose(pFile);
    readWave();
    Full = true;
}

trackComplex Wave::int2Complex(int channel)
{

    long length = Samples;
    //int log = log2(Samples);
    //length = (log2f32(Samples)-log) > 0 ? length<<=(log+1) : length<<=log;
    trackComplex aux = new complex<double>[length];

    cout << "tamaño " << length << endl;
    for (int i = 0; i < Samples; i++)
    {
        aux[i] = (double)data[channel][i];
        //cout << aux[i] ;
    }
    lenComplex = length;

    return aux;
}

Wave::~Wave()
{
    if (Full)
    {
        Full = false;
        delete[] data[0];
        if (2 == wChannels)
            delete[] data[1];
        delete[] data;
    }
}

void Wave::readWave()
{
    FILE *pFile;
    long pos = 0;
    int sampleLeft = 0, sampleRight = 0, i;
    trackInt trackL, trackR;
    string name = PATH + nameWave + ".wav";
    const char *punter = name.c_str();

    pFile = fopen(punter, "r");

    if (pFile == NULL)
    {
        fprintf(stderr, "No pude abrir el archivo %s\n", punter);
        exit(0);
    }

    //cout<<"W=[ ";
    fseek(pFile, 44, SEEK_SET); //Posiciono el puntero al archivo despues de la cabecera.
    if (1 == wChannels)
    {
        i = 0;
        data = new int *[1];
        trackL = new int[Samples];
        while (pos < signalSize)
        { //reviso el fin de archivo
            fread(&sampleLeft, BytePorMu, 1, pFile);
            trackL[i] = (short)sampleLeft;
            //cout<<sampleLeft<<";"<<endl;
            averageL += sampleLeft * sampleLeft;
            pos += BytePorMu;
            i++;
        }
        data[0] = trackL;
        averageL /= i / BytePorMu;
    }
    else if (2 == wChannels)
    {
        i = 0;
        Samples >>= 1;
        trackL = new int[Samples];
        trackR = new int[Samples];
        data = new int *[2];

        while (pos < signalSize)
        { //reviso el fin de archivo
            fread(&sampleLeft, BytePorMu, 1, pFile);
            fread(&sampleRight, BytePorMu, 1, pFile);

            trackL[i] = (short)sampleLeft;
            trackR[i] = (short)sampleRight;
            averageL += sampleLeft * sampleLeft;
            averageR += sampleRight * sampleRight;
            pos += BytePorMu << 1;
            i++;
        }

        data[0] = trackL;
        data[1] = trackR;
        averageL /= i / BytePorMu;
        averageR /= i / BytePorMu;
    }
    //cout<<"];"<<endl;

    // printf("E %d \n\n",i);
}

void Wave::printTrack(int channel)
{
    cout << "W=[ ";
    for (int i = 0; i < Samples; i++)
    {
        if (2 == BytePorMu)
            cout << (short)data[channel][i] << " ";
        else
            cout << data[channel][i] << " ";
    }
    cout << " ];" << endl;
}

void Wave::newTrack(int length, trackComplex spectre, int channel)
{
    //FFT(spectre, length, 1);
    for (int j = 0; j < Samples; j++)
    {
        data[channel][j] = (int)spectre[j].real();
    }
}

void Wave::newTrack(int marcos, int frameSize, matrizComplex spectre, int channel)
{

    int avance = 0;

    cout << "Kesima " << Samples << endl;
    for (int i = 0; i < marcos; i++)
    {
        FFT(spectre[i], frameSize, 1);
        for (int j = 0; j < frameSize; j++)
        {
            if (avance + j < Samples)
            {
                data[channel][avance + j] = (int)spectre[i][j].real();
            }
        }
        avance += frameSize;
    }
}

void Wave::newTrack(int length, trackDouble chanTrack, int channel)
{

    for (int j = 0; j < length; j++)
    {
        data[channel][j] = (int)chanTrack[j];
    }
}

void Wave::writeWave(string add)
{

    FILE *pFile;
    add += ".wav";
    nameWave += add;
    char aux;
    short auxi, auxr;
    const char *punter = nameWave.c_str();

    pFile = fopen(punter, "w");

    if (pFile == NULL)
    {
        fprintf(stderr, "No pude escribir el archivo %s\n", punter);
        exit(0);
    }

    fwrite(&groupID, 4, 1, pFile);
    fwrite(&fileSize, 4, 1, pFile);
    fwrite(&riffType, 4, 1, pFile);
    fwrite(&chunkID, 4, 1, pFile);
    fwrite(&chunkSize, 4, 1, pFile);
    fwrite(&wFormatTag, 2, 1, pFile);
    fwrite(&wChannels, 2, 1, pFile);
    fwrite(&dwSamplesPerSec, 4, 1, pFile);
    fwrite(&dwAvgBytesPerSec, 4, 1, pFile);
    fwrite(&wBlockAlign, 2, 1, pFile);
    fwrite(&wBitsPerSample, 2, 1, pFile);
    fwrite(&dataID, 4, 1, pFile);
    fwrite(&signalSize, 4, 1, pFile);
    cout << "length: " << Samples * BytePorMu << endl;

    for (int i = 0; i < Samples; i++)
    {
        auxi = (short)data[0][i];
        fwrite(&auxi, BytePorMu, 1, pFile);
        if (2 == wChannels)
        {
            auxr = (short)data[1][i];
            fwrite(&auxr, BytePorMu, 1, pFile);
        }
    }

    fclose(pFile);
}

#endif