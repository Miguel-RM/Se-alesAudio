#include<iostream>
#include<stdlib.h>
#include"estructuras.cpp"
#include"Fourier.cpp"
#include"tracks.cpp"

using namespace::std;
using namespace::audio;
using namespace::Fourier;
using namespace::Tracks;


#ifndef WAVE_CPP
#define WAVE_CPP


class Wave
{

friend class Spectrum;
private:
    /* data */
    unsigned char  groupID[5]; /* dede ser 'RIFF' */
    unsigned int   fileSize; /* sin contar los ultimos 15 bytes*/
    unsigned char  riffType[5]; /* debe ser 'WAVE' */ 
    unsigned char  chunkID[5]; /* debe ser 'fmt ' */ 
    unsigned int   chunkSize; /* tamano del EWavezado que falta por leer*/
    unsigned short wFormatTag; /* debe ser 1 (sin compresion) */
    unsigned short wChannels;
    unsigned int   dwSamplesPerSec; /*regimen de muestreo */
    unsigned int   dwAvgBytesPerSec; /* Para que el player sepa tam de buffer*/
    unsigned short wBlockAlign; /* wChannels)*(wBitsPerSample/8) */
    unsigned short wBitsPerSample;
    unsigned char  dataID[5]; /* debe ser 'data' */
    unsigned int   signalSize; /* tamano de la senal en muestras */
    unsigned int   BytePorMu;
    Matrix32 data; // el audio
    double averageL; // La energia promedio del audio
    double averageR;
    string nameWave; //path y nombre del audio
    void readWave();

public:

    Wave(string Name);
    void printTrack(int channel);
    void newTrack(MatrixFloat &spectreR, MatrixFloat &spectreI, int channel);
    void writeWave(string add);
    int getSamplesPerSec();
    ~Wave();
};

int Wave::getSamplesPerSec()
{
    return dwSamplesPerSec;
}

Wave::Wave(string Name)
{
    int i;
    FILE *pFile;
    nameWave = Name;
    Name+=".wav";
    const char *punter=Name.c_str();

    pFile=fopen(punter,"r");

    if (pFile==NULL) {
        fprintf(stderr,"No pude abrir el archivo %s\n",punter);
        exit(0);
    }


    fread(&groupID,4 , 1 , pFile);groupID[4]=0;
    fread(&fileSize , 4, 1 , pFile);
    fread(&riffType, 4, 1, pFile); riffType[4]=0;
    fread(&chunkID , 4, 1 , pFile);
    fread(&chunkSize , 4, 1 , pFile);
    fread(&wFormatTag , 2, 1 , pFile);
    fread(&wChannels , 2 , 1 , pFile);
    fread(&dwSamplesPerSec , 4, 1 , pFile);
    fread(&dwAvgBytesPerSec , 4 , 1, pFile);
    fread(&wBlockAlign , 2 , 1, pFile);
    fread(&wBitsPerSample , 2 , 1, pFile);
    fread(&dataID , 4 , 1 , pFile); dataID[4]=0;
    fread(&signalSize , 4 , 1 , pFile);
    
    
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
    
    
    BytePorMu=wBitsPerSample/8;
    fclose(pFile);
    readWave();
}

Wave::~Wave()
{
}

void Wave::readWave()
{
   FILE *pFile;
   long pos=0;
   int sampleLeft = 0,sampleRight = 0;
   track32 trackauxL,trackauxR;
   string name = nameWave + ".wav";
   const char *punter=name.c_str();

    pFile=fopen(punter,"r");
   
    if (pFile==NULL) {
        fprintf(stderr,"No pude abrir el archivo %s\n",punter);
        exit(0);
    }

    //cout<<"W=[ ";
    fseek(pFile , 44 , SEEK_SET);  //Posiciono el puntero al archivo despues de la cabecera.   
    if(1 == wChannels)
    {
        while(pos<signalSize){  //reviso el fin de archivo
            fread(&sampleLeft,BytePorMu,1,pFile);
            trackauxL.push_back(sampleLeft);
            //cout<<sampleLeft<<";"<<endl;
            averageL+=sampleLeft*sampleLeft;
            pos+=BytePorMu;
        }
        data.push_back(trackauxL);
    }else if(2 == wChannels)
    {
        while(pos<signalSize){  //reviso el fin de archivo
            fread(&sampleLeft,BytePorMu,1,pFile);
            fread(&sampleRight,BytePorMu,1,pFile);
            
            trackauxL.push_back(sampleLeft);
            trackauxR.push_back(sampleRight);
            //cout<<sampleLeft<<";"<<endl;
            averageL+=sampleLeft*sampleLeft;
            averageR+=sampleRight*sampleRight;
            pos+=BytePorMu;
        }
        data.push_back(trackauxL);
        data.push_back(trackauxR);
    }
   //cout<<"];"<<endl;
   averageL/=data[0].size()/BytePorMu;

   //printf("E %f \n\n",average);
   cout<<"Signal: "<<signalSize<<endl;
}

void Wave::printTrack(int channel)
{
    cout<<"W=[ ";
    for (int i = 0; i < data[channel].size(); i++)
    {
        if(2==BytePorMu)
            cout << (short)data[channel][i] << "; ";
        else 
            cout << data[channel][i] << "; ";
    }
    cout<<" ];"<<endl;
    
}

void Wave::newTrack(MatrixFloat &spectreR, MatrixFloat &spectreI, int channel)
{
    data[channel].clear();
    int frameSize = spectreR[0].size();
    trackFloat salidaR, salidaI;

    for (int i = 0; i < spectreR.size(); i++)
    {
        salidaR = salidaI = generaCeros(frameSize);
        tRF1D(spectreR[i], spectreI[i], salidaR, salidaI, 1);
        for (int j = 0; j < frameSize; j++)
        {
            data[channel].push_back((int)round(salidaR[j]));
        }
    }
    //signalSize = data.size()*BytePorMu;
}

void Wave::writeWave(string add)
{

    FILE *pFile;
    add+=".wav";
    nameWave+=add;
    char aux;
    int auxi, auxr;
    const char *punter=nameWave.c_str();

    pFile=fopen(punter,"w");

    if (pFile==NULL) {
        fprintf(stderr,"No pude escribir el archivo %s\n",punter);
        exit(0);
    }

    
    fwrite(&groupID,4 , 1 , pFile);
    fwrite(&fileSize , 4, 1 , pFile);
    fwrite(&riffType, 4, 1, pFile);
    fwrite(&chunkID , 4, 1 , pFile);
    fwrite(&chunkSize , 4, 1 , pFile);
    fwrite(&wFormatTag , 2, 1 , pFile);
    fwrite(&wChannels , 2 , 1 , pFile);
    fwrite(&dwSamplesPerSec , 4, 1 , pFile);
    fwrite(&dwAvgBytesPerSec , 4 , 1, pFile);
    fwrite(&wBlockAlign , 2 , 1, pFile);
    fwrite(&wBitsPerSample , 2 , 1, pFile);
    fwrite(&dataID , 4 , 1 , pFile);
    fwrite(&signalSize , 4 , 1 , pFile);
    cout << "length: "<<BytePorMu<<endl;

    for (int i = 0; i < data[0].size(); i++)
    {
        auxi = data[0][i];
        fwrite(&auxi,BytePorMu , 1 , pFile);
        if(2==wChannels){
            auxr = data[1][i];
            fwrite(&auxr,BytePorMu , 1 , pFile);
        }
    }
    
    fclose(pFile);
}

#endif