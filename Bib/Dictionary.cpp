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
    void spectrogram();
    void save();
    ~Dictionary();
};

void Dictionary::spectrogram()
{
    for (int i = 0; i < dictionary.size(); i++)
    {
        dictionary[i]->createPPM();
    }
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