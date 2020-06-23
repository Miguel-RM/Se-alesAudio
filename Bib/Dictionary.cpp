#include <iostream>
#include <fstream>
#include "estructuras.cpp"
#include "Espectro.cpp"

#ifndef DICTIONARY_CPP
#define DICTIONARY_CPP

class Dictionary
{
private:
    vector<Spectrum *> dictionary;

public:
    Dictionary(/* args */);
    void add(Spectrum *A);
    void remove(int index);
    void search(Spectrum *A, int &pos, string &name, double &distance);
    double insertN(vector<NN> &list, NN Nei, int k);
    void KNN(Spectrum *A, int k);
    int spectrogram();
    void save();
    ~Dictionary();
};

double Dictionary::insertN(vector<NN> &list, NN Nei, int k)
{
    int pos = list.size();
    double max;
    for (int i = 0; i < list.size(); i++)
    {
        if(Nei.dist < list[i].dist)
        {
            pos = i;
            break;
        }
    }
    list.insert(list.begin()+pos, Nei);

    if(list.size() > k)
        list.pop_back();

    max = list[list.size()-1].dist;
    
    return max;  
}


void Dictionary::KNN(Spectrum *A, int k)
{
    double MaxDist = 9e20;
    double d;
    vector<NN> list;
    NN aux;
    int MaxV = -1;
    int pos;

    // Busqueda de los KNN

    for (int i = 0; i < dictionary.size(); i++)
    {
        d = dictionary[i]->DTWSandC(*A);
        if (d < MaxDist || list.size() < k)
        {
            aux.dist=d;
            aux.label = dictionary[i]->getName();
            aux.votes = 1;
            MaxDist = insertN(list, aux, k);
        }
    }
    
    // votación de los KNN

    for (int i = 0; i < list.size(); i++)
    {
        for (int j = i+1; j < list.size(); j++)
        {
            if(0 == list[i].label.compare(list[j].label))
            {
                list[i].votes++;
                list.erase(list.begin()+j);
            }
        }
    }

    // decision 

    for (int i = 0; i < list.size(); i++)
    {
        //cout << "Palabra: "<< list[i].label << endl;
        //cout << "Votos: " << list[i].votes << endl;
        if(MaxV < list[i].votes)
        {
            MaxV = list[i].votes;
            pos = i;
        }
    }
    cout << "Palabra: "<< list[pos].label << endl;
    cout << "Votos: " << MaxV << endl;
    
}

int Dictionary::spectrogram()
{
    if(dictionary.size() > 0 && dictionary[0]->getFornt())
     return -1;

    for (int i = 0; i < dictionary.size(); i++)
    {
        dictionary[i]->createPPM();
    }
    return 0;
}

void Dictionary::search(Spectrum *A, int &pos, string &name, double &distance)
{
    double dist;
    distance = 9e16;
    pos = -1;
    for (int i = 0; i < dictionary.size(); i++)
    {
        dist = dictionary[i]->DTWSandC(*A);
        cout << "d: " << dist << " N: " << dictionary[i]->getName() << endl;
        if (distance > dist)
        {
            distance = dist;
            pos = i;
        }
    }
    if (-1 != pos)
        name = dictionary[pos]->getName();
    else
    {
        name = "No Match";
        distance = -1;
    }
}

void Dictionary::add(Spectrum *A)
{
    dictionary.push_back(A);
}

void Dictionary::save()
{
    string name = "dictionary/conf";
    ofstream conf(name);
    if (conf.bad())
    {
        cout << "Wrong to config File" << endl;
        exit(0);
    }
    conf << dictionary.size() << endl;

    for (int i = 0; i < dictionary.size(); i++)
    {
        conf << dictionary[i]->name << endl;
        dictionary[i]->save();
    }
    conf.close();
}

Dictionary::Dictionary()
{
    int length;
    string name = "conf";
    string file;
    Spectrum *aux;

    name = PATHDICT + name;
    ifstream conf(name);
    ifstream sp;

    if (conf.is_open() && conf.peek() != -1)
    {

        conf >> length;

        for (int i = 0; i < length; i++)
        {
            conf >> file;
            cout << file << endl;
            aux = new Spectrum(file);
            dictionary.push_back(aux);
        }

        conf.close();
        cout << "Length of dictionary: " << length << endl;
    }
}

Dictionary::~Dictionary()
{
    if (0 < dictionary.size())
        save();
}

#endif