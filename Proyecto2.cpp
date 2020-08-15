#include<iostream>
#include"Bib/Filtros.cpp"
#include"Bib/Wave.cpp"

using namespace::std;
using namespace::Filters;

void bancoFIR(MatrizDouble &bankA, MatrizDouble &bankB);
void bancoIIR(MatrizDouble &bankA, MatrizDouble &bankB);

int main()
{

    MatrizDouble bankA;
    MatrizDouble bankB;
    trackDouble weight;


    bancoFIR(bankA, bankB);
    //bancoIIR(bankA, bankB);
    weight = new double[5];
    weight[0]= 0.1;
    weight[1]= 0.2;
    weight[2]= 0.0;
    weight[3]= 0.0;
    weight[4]= 0.0;


    cout << "leyendo archivo: meg" << ".wav"<<endl;
    Wave audio("Monst"); 
    int lengthT = audio.getSamples();
    trackInt channel0, channel1;
    trackDouble y0 = new double[lengthT];
    trackDouble y1 = new double[lengthT];

    channel0 = audio.getTrack(0);
    channel1 = audio.getTrack(1);

    cout << "Mezclando ..." <<endl;
    cout << "   Canal 1" <<endl;
    y0 = Mixer(lengthT, channel0, bankA, bankB, weight);
    
    cout << "   Canal 2" <<endl;
    y1 = Mixer(lengthT, channel1, bankA, bankB, weight);
    //applyFilter(lengthT, a, h, channel0, y0);
    //applyFilter(lengthT, a, h, channel1, y1);
    

    cout << "Sintetizando ..." << endl;
    audio.newTrack(lengthT, y0, 0);
    audio.newTrack(lengthT, y1, 1);

    audio.writeWave("altas");

    cout << "Proceso terminado"<< endl;
    
    return 0;
}

void bancoIIR(MatrizDouble &bankA, MatrizDouble &bankB)
{
    trackFloat b;
    trackFloat a;
}


void bancoFIR(MatrizDouble &bankA, MatrizDouble &bankB)
{
    hr aux;
    Hr data;
    trackFloat h;
    trackFloat a;
   
    // Filtro de 20Hz a 60Hz

    aux = {0.4,2}; data.push_back(aux);
    aux = {1.0, 3}; data.push_back(aux);
    aux = {1.0, 3}; data.push_back(aux);
    aux = {0.4, 4}; data.push_back(aux);
    h = coefFIR(data,2204);
    bankB.push_back(h);
    bankA.push_back(a);

    // filtro de 60Hz a 250Hz

    data.clear();
    aux = {0.4,6}; data.push_back(aux);
    aux = {1.0, 7}; data.push_back(aux);
    aux = {1.0, 13}; data.push_back(aux);
    aux = {0.4, 14}; data.push_back(aux);
    h = coefFIR(data,1470);
    bankB.push_back(h);
    bankA.push_back(a);

    // filtro de 250Hz a 2kHz

    data.clear();
    aux = {0.4,18}; data.push_back(aux); //9 10 35 36
    aux = {1.0, 19}; data.push_back(aux);
    aux = {1.0, 129}; data.push_back(aux);
    aux = {0.4, 130}; data.push_back(aux);
    h = coefFIR(data,440);
    bankB.push_back(h);
    bankA.push_back(a);

    // filtro de 2kHz a 6kHz

    data.clear();
    aux = {0.4, 42}; data.push_back(aux);
    aux = {1.0, 43}; data.push_back(aux);
    aux = {1.0, 118}; data.push_back(aux);
    aux = {0.4, 119}; data.push_back(aux);
    h = coefFIR(data,440);
    bankB.push_back(h);
    bankA.push_back(a);

    // filtro de 6kHz a 16kHz

    data.clear();
    aux = {0.4,61}; data.push_back(aux);
    aux = {1.0, 62}; data.push_back(aux);
    aux = {1.0, 158}; data.push_back(aux);
    aux = {0.4, 159}; data.push_back(aux);
    h = coefFIR(data,220);
    bankB.push_back(h);
    bankA.push_back(a);//*/
}
