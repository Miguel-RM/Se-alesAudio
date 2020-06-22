#include "Bib/Wave.cpp"
#include "Bib/Fourier.cpp"
#include "Bib/Filtros.cpp"
#include "Bib/tracks.cpp"
#include "Bib/Espectro.cpp"
#include "Bib/TimeTecnics.cpp"
#include "Bib/Dictionary.cpp"
#include "Bib/Homomorphic.cpp"
#include <stdlib.h>
#include <limits>

using namespace ::std;
using namespace ::Filters;

void menu()
{
    cout << endl << "Identificador de Palabras" << endl;
    cout << "opciones: " << endl;
    cout << "a) Guardar lista de archivos de audio" << endl;
    cout << "b) Guardar palabra" << endl;
    cout << "c) Reconocer palabra" << endl;
    cout << "e) Salir del programa" << endl << endl;
    
}

void loadList(Dictionary &dataBase, string namelist)
{
    int length;
    string file;
    string label;
    char chain[80];
    Spectrum *aux;
    ifstream lista(namelist);
    ifstream sp;
    Wave audio;

    if (lista.is_open() && lista.peek() != -1)
    {

        //lista >> length;
        lista.getline(chain, 80, '\n');
        length = atoi(chain);
        

        for (int i = 0; i < length; i++)
        {
            //lista >> file;
            lista.getline(chain, 80, '\n');
            file = chain;
            cout << file << endl;
            lista.getline(chain, 80, '\n');
            label = chain;
            audio.loadWave(file);
            aux = new Spectrum(audio, label, 0, true);
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
    string label;
    char Nam[80];
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
        cin.ignore(std::numeric_limits<int>::max(),'\n');
        cout << "caracter " << c << endl;
        
        if('a' == c)
        {
            system("clear");
            menu();
            cout << "\tNombre del archivo lista: ";
            scanf("%[^\n]",Nam);
            nombre = Nam;

            loadList(dataBase, nombre);

            cin.ignore(std::numeric_limits<int>::max(),'\n');
            
        }else if('b' == c)
        { 
            system("clear");
            menu();
            cout << "Nombre del audio: ";
            scanf("%[^\n]",Nam);
            nombre = Nam;
            cin.ignore(std::numeric_limits<int>::max(),'\n');
            cout << "\tEtiqueta: ";
            scanf("%[^\n]",Nam);            
            label = Nam;

            audio.loadWave(nombre);
            sp = new Spectrum(audio, label, 0, true);
            dataBase.add(sp);

            cout << "Palabra guardada"<< endl;
            cin.ignore(std::numeric_limits<int>::max(),'\n');

        }else if('c' == c)
        {
            system("clear");
            menu();
            cout << "\tNombre del audio: ";
            scanf("%[^\n]",Nam);
            nombre = Nam;
            label = "Unknown";

            audio.loadWave(nombre);
            sp = new Spectrum(audio, label, 0, true);
            dataBase.KNN(sp, 3);

            cin.ignore(std::numeric_limits<int>::max(),'\n');
        }else if('i' == c)
        {
            system("clear");
            menu();
            if(0 == dataBase.spectrogram())
                cout << "Espectrogramas creados"<< endl;
            else
                cout << "Solo con escala de Bark"<< endl;
            
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