#ifndef klasa_h
#define klasa_h
#define ROZMIAR_PROBLEMU 60
#define POCZATKOWA_POPULACJA 300 // 300
#define WYBRANI 150 //50

/* WYBRANI - potomkowieNaOsobnika
30-33	10
34-37	9
38-42	8
43-49	7
50-59	6
60-74	5
75-100	4
*/


#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <cmath>

using namespace std;

class Osobnik // pojedynczy osobnik, który wchodzi w sklad populacji
{
private:
    public:
    int geny[ROZMIAR_PROBLEMU]; // rozwiazanie problemu, i-miasta, g[i]-atrakcje
    int wynik; // funkcja celu
public:
    Osobnik(); // tworzenie osobnika z losowym rozwiazaniem
    Osobnik(int* geny_); // tworzenie osobnika z rozwiazaniem podanym argumentem
    ~Osobnik();
    void ocena(); // wyliczanie funkcji celu
    void mutuj(); // mutacja osobnika
    int* getGeny(); // zwrocenie wskaznika na tablice genów
    int getWynik(); // zwrocenie wyniku
};

class Gatunek
{
private:
    public:
    vector <Osobnik> populacja; // calosc populacji w jednej generacji
    vector <Osobnik> dzieci; // dzieci stworzone z krzyzowania
    vector <Osobnik> wybrani;
    //vector <int> historiaOceny; //

public:
    Gatunek(); // utworzenie losowej populacji o rozmiarze POCZATKOWA_POPULACJA
    ~Gatunek();
    void krzyzowanieOX(); // tworzy jedno dziecko
    void krzyzowanieCX(); // tworzy dwójkę dzieci
    void krzyzowaniePMX(); // tworzy dwójkę dzieci
    void ustalenieWspolczynnikaMutacji();
    void mutacja(); // mutuje dzieci po krzyzowaniu
    void ocena(); // wywoluje funkcje ocena dla wszystkich dzieci
    void selekcja(); // wybiera najlepszych posród dzieci do nowego wectora populacji
    void selekcjaRankEli(); // selekcja elitarna
    bool sprawdzaniePoprawnosciPopulacji();
    void rysujTabele(); // rysuje tablice miast i flow
    void zapisDoPliku();
};



#endif // klasa_h
