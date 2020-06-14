#include "Bib/Wave.cpp"
#include "Bib/Fourier.cpp"
#include "Bib/Filtros.cpp"
#include "Bib/tracks.cpp"
#include "Bib/Espectro.cpp"
#include "Bib/TimeTecnics.cpp"
#include "Bib/Dictionary.cpp"
#include "Bib/Homomorphic.cpp"

using namespace ::std;
using namespace ::Filters;

void menu()
{
    cout << endl << "Identificador de Palabras" << endl;
    cout << "opciones: " << endl;
    cout << "a) Guardar lista de archivos de audio" << endl;
    cout << "b) Guardar palabra" << endl;
    cout << "c) Reconocer palabra" << endl << endl;
    
}

void loadList(Dictionary &dataBase, string namelist)
{
    int length;
    string file;
    Spectrum *aux;
    ifstream lista(namelist);
    ifstream sp;
    Wave audio;

    if (lista.is_open() && lista.peek() != -1)
    {

        lista >> length;

        for (int i = 0; i < length; i++)
        {
            lista >> file;
            cout << file << endl;
            audio.loadWave(file);
            aux = new Spectrum(audio, 0);
            dataBase.add(aux);
        }

        lista.close();
        cout << "Archivos cargados al diccionario: " << length << endl;
    }
}

int main()
{

    Dictionary dataBase;
    string nombre;
    Wave audio;
    Spectrum *sp;
    
    char c;  
    int pos;
    double distancia;

    system("clear");
    menu();
    while (true)
    {
        
        cout << "Opción: ";
        cin >> c;
        if('a' == c)
        {
            system("clear");
            menu();
            cout << "\tNombre del archivo lista: ";
            cin >> nombre;
            loadList(dataBase, nombre);
            
        }else if('b' == c)
        { 
            system("clear");
            menu();
            cout << "\tNombre del audio: ";
            cin >> nombre;
            audio.loadWave(nombre);
            sp = new Spectrum(audio, 0);
            dataBase.add(sp);
            cout << "Palabra guardada"<< endl;

        }else if('c' == c)
        {
            system("clear");
            menu();
            cout << "\tNombre del audio: ";
            cin >> nombre;
            audio.loadWave(nombre);
            sp = new Spectrum(audio, 0);
            dataBase.search(sp, pos, nombre, distancia);

            cout << "Palabra: "<< nombre << endl;
            cout << "Distancia: " << distancia << endl;
        }else if('i' == c)
        {
            system("clear");
            menu();
            dataBase.spectrogram();
            cout << "Espectrogramas creados"<< endl;
            
        }else if('e' == c)
        {
            return 0;
        }else
        {
            system("clear");
            menu();
            cout << "Opción erronea"<< endl;
        }
        
    }

    return 0;
}