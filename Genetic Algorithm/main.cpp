#include <iostream>
#include "klasa.h"

using namespace std;

extern int wielkoscMutacji;
extern int szansaNaMutacje;
extern int miejscePrzeciecia;

int main()
{
    srand (time(NULL));

    int liczbaIteracji=1;
    long int sum=0;
    int min=4000000;
    int max=0;

    std::fstream plik;
    plik.open( "ocena.txt", ios::out );
    std::fstream plik2;
    plik2.open( "gain.txt", ios::out );


    if( plik.good() == true )
    {
        for (int iter=0; iter<liczbaIteracji; iter++)
        {
            cout << iter << endl;
            Gatunek p1;
            cout << "Pierwsza epoka: " << p1.populacja[0].getWynik() << endl;
            int pierwszaIter=p1.populacja[0].getWynik();
            for (int i=0; i<299; i++)
            {
                p1.selekcjaRankEli();
                p1.krzyzowanieCX();
                p1.ustalenieWspolczynnikaMutacji();
                p1.mutacja();
                p1.ocena();
                if (p1.sprawdzaniePoprawnosciPopulacji()==false)
                    exit(0);

                p1.zapisDoPliku();
            }
            cout << "Ostatnia epoka: " << p1.populacja[0].getWynik() << endl;
            sum+=p1.populacja[0].wynik;
            if (p1.populacja[0].wynik<min)
                min=p1.populacja[0].wynik;
            if (p1.populacja[0].wynik>max)
                max=p1.populacja[0].wynik;
            plik << p1.populacja[0].wynik << endl;
            plik2 << 100*(double(pierwszaIter-p1.populacja[0].wynik)/double(pierwszaIter)) << endl;
        }
        cin >> liczbaIteracji;
    }
    cout << endl << "srednia: " << sum/liczbaIteracji << endl;
    cout << "min: " << min << endl;
    cout << "max: " << max << endl;





    //p1.rysujTabele();
    clock_t s, f;
    double czas=0;
    s = clock();
    f = clock();
    czas = (double)(f - s) / (double)(CLOCKS_PER_SEC);
    //cout << czas << endl ;

    return 0;
}
