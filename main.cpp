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


    //bancoFIR(bankA, bankB);
    bancoIIR(bankA, bankB);
    weight = new double[5];
    weight[0]= 1.0;
    weight[1]= 1.0;
    weight[2]= 1.0;
    weight[3]= 1.0;
    weight[4]= 1;


    cout << "leyendo archivo: meg" << ".wav"<<endl;
    Wave audio("meg"); 
    int lengthT = audio.getSamples();
    trackInt channel0, channel1;
    trackDouble y0 = new double[lengthT];
    trackDouble y1 = new double[lengthT];

    channel0 = audio.getTrack(0);
    channel1 = audio.getTrack(1);

    cout << "Mezclando ..." <<endl;
    cout << "   Canal 1" <<endl;
    y0 = Mixer(lengthT, channel0, bankA, bankB, weight);
    for (int i = 0; i < lengthT; i++)
    {
        cout << y0[i] << " ";
    }cout << endl;
    
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

    /* Filtro I 
    b = {1.59371e18, 4.33381e16, 8.58386e14, 1.16022e13,
     1.26943e11, 1.1005e9,  7.98271e6, 47661.2, 240.397,
     1.00606, 0.00355687, 0.0000103506, 2.52026e-8, 4.86222e-11,
     7.59341e-14, 8.0925e-17, 6.28176e-20};
    a = {0, 0, 0, 0, 0, 0, 0, 1};
    bankB.push_back(b); b.clear();
    bankA.push_back(a);a.clear();*/
    
    
    // Filtro II 
    b =  {1.80676e-12, 1.69083e-10, 5.24146e-8, 2.96158e-6,
     0.000430347, 0.0113981, 1};
    a =  {0, 0, 1.85099e-7};
    bankB.push_back(b); b.clear();
    bankA.push_back(a);a.clear();
    // Filtro III 
   /* b =  {1.07868e26, 3.07996e23, 4.83432e20, 5.16542e17,
     4.08208e14, 2.46165e11,  1.14115e8, 40266.2, 10.5913,
     0.00203991, 2.92877e-7, 3.20064e-11, 2.68882e-15, 1.72368e-19,
     8.17253e-24, 2.63777e-28, 4.68012e-33};
    a =  {0, 0, 0, 0, 0, 0, 0, 1};
    bankB.push_back(b); b.clear();
    bankA.push_back(a);a.clear();
    // Filtro IV 
    b =  {1.59371e34, 4.33381e30, 8.58386e26, 1.16022e23,
     1.26943e19, 1.1005e15,  7.98271e10, 4.76612e6, 240.397,
     0.0100606, 3.55687e-7, 1.03506e-11, 2.52026e-16, 4.86222e-21,
     7.59341e-26, 8.0925e-31, 6.28176e-36};
    a =  {0, 0, 0, 0, 0, 0, 0, 1};
    bankB.push_back(b); b.clear();
    bankA.push_back(a);a.clear();
    // Filtro V 
    b =  {1.7523e38, 1.48908e34, 1.0026e30, 4.49468e25,
     1.68331e21, 4.95781e16,  1.2477e12, 2.58225e7, 460.181,
     0.00681345, 8.6866e-8, 9.10746e-13, 8.15908e-18, 5.74836e-23,
     3.3833e-28, 1.32587e-33, 4.11681e-39};
    a =  {0, 0, 0, 0, 0, 0, 0, 1};
    bankB.push_back(b); b.clear();
    bankA.push_back(a);a.clear();*/
    

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
