#include "Bib/Wave.cpp"
#include "Bib/Fourier.cpp"
#include "Bib/Filtros.cpp"
#include "Bib/tracks.cpp"
#include "Bib/Espectro.cpp"
#include "Bib/TimeTecnics.cpp"
#include "Bib/Dictionary.cpp"

using namespace ::std;
using namespace ::Filters;

int main()
{
    Wave audio("audio");
    Spectrum spec;
    Spectrum specII(audio, 0);

    //specII.save();

   
    Dictionary D;
    int pos;
    
    //specII.createPPM("audio1");
    //specII.save();
    //D.add(spec);
    //D.add(specII);

    double d;
    D.search(specII,pos, d);

    cout << "Distancia: "<<d <<" position "<<pos<<endl;

    return 0;
}