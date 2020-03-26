#include "Wave.cpp"

using namespace::std;

int main()
{
    Wave audio("audio.wav");
    cout << "Tamaño: " << sizeof(unsigned char) << endl;
    audio.printTrack();
    return 0;
}