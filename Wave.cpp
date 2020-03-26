#include<iostream>
#include<stdlib.h>
#include<vector>

using namespace::std;


#ifndef WAVE_CPP
#define WAVE_CPP

typedef vector<short> track;

class Wave
{
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
    double average; // La energia promedio del audio
    track data; // el audio
    string nameWave; //path y nombre del audio
    void readWave();

public:
    Wave(string Name);
    void printTrack();
    void writeWave();
    ~Wave();
};

Wave::Wave(string Name)
{
    int i;
    FILE *pFile;
    const char *punter=Name.c_str();

    pFile=fopen(punter,"r");
    nameWave = Name;

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
   short sampleLeft = 0,sampleRight = 0;
   const char *punter=nameWave.c_str();

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
            data.push_back(sampleLeft);
            //cout<<sampleLeft<<";"<<endl;
            average+=sampleLeft*sampleLeft;
            pos+=BytePorMu;
        }
    }else if(2 == wChannels)
    {
        while(pos<signalSize){  //reviso el fin de archivo
            fread(&sampleLeft,BytePorMu,1,pFile);
            fread(&sampleRight,BytePorMu,1,pFile);
            sampleLeft += sampleRight;
            sampleLeft /= 2;
            data.push_back(sampleLeft);
            //cout<<sampleLeft<<";"<<endl;
            average+=sampleLeft*sampleLeft;
            pos+=BytePorMu;
        }
    }
   //cout<<"];"<<endl;
   average/=((signalSize-44)/BytePorMu);

   printf("E %f \n\n",average);
   cout<<"Tam: "<<data.size()<<endl;
}

void Wave::printTrack()
{
    cout<<"W=[ ";
    for (int i = 0; i < data.size(); i++)
    {
        cout << data[i] << "; ";
    }
    cout<<" ];"<<endl;
    
}

#endif