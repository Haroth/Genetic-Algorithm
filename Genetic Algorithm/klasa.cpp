#include "klasa.h"

int miasta[ROZMIAR_PROBLEMU][ROZMIAR_PROBLEMU];
int flow[ROZMIAR_PROBLEMU][ROZMIAR_PROBLEMU];
int szansaNaMutacje=17; // 1000 to 100%
int progPodobienstwa=30;


//!===================================================================================================================================================================================================
//!-------------------------------------------------------------- OSOBNIK ----------------------------------------------------------------------------------------------------------------------------
//!===================================================================================================================================================================================================

Osobnik::Osobnik()
{
    vector <int> vec;
    int losowa;
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
        vec.push_back( i );
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
    {
        losowa=rand()%(ROZMIAR_PROBLEMU-i);
        geny[i]=vec[losowa];
        vec.erase(vec.begin()+losowa);
    }
}

Osobnik::Osobnik(int* geny_)
{
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
        geny[i]=geny_[i];
}



Osobnik::~Osobnik() {}

void Osobnik::ocena()
{
    wynik=0;
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
        for (int j=0; j<ROZMIAR_PROBLEMU; j++)
            wynik+=miasta[i][j]*flow[geny[i]][geny[j]];
}

void Osobnik::mutuj()
{
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
    {
        if ((rand()%1000)<szansaNaMutacje)
        {
            int losowa=rand()%ROZMIAR_PROBLEMU;
            int losowa2=rand()%ROZMIAR_PROBLEMU;
            while (losowa==losowa2)
                losowa2=rand()%ROZMIAR_PROBLEMU;
            int pom=geny[losowa];
            geny[losowa]=geny[losowa2];
            geny[losowa2]=pom;
        }

    }
}

int* Osobnik::getGeny()
{
    return geny;
}

int Osobnik::getWynik()
{
    return wynik;
}

//!===================================================================================================================================================================================================
//!-------------------------------------------------------------- POPULACJA --------------------------------------------------------------------------------------------------------------------------
//!===================================================================================================================================================================================================

Gatunek::Gatunek()
{
    ifstream plik;
    plik.open( "problemy\\lipa60b.dat", ios::in);
    if( plik.good() == true )
    {
        int pom;
        plik >> pom;

        for (int i=0; i<ROZMIAR_PROBLEMU; i++)
            for (int j=0; j<ROZMIAR_PROBLEMU; j++)
                plik >> flow[i][j];
        for (int i=0; i<ROZMIAR_PROBLEMU; i++)
            for (int j=0; j<ROZMIAR_PROBLEMU; j++)
                plik >> miasta[i][j];
    }
    else
        cout << "Plik nie zostal wczytany" << endl;
    plik.close();

    for (int i=0; i<POCZATKOWA_POPULACJA; i++)
        populacja.push_back(Osobnik());

    ocena();
}

Gatunek::~Gatunek() {}


void Gatunek::selekcja()
{

    //100*e^(-0.024*x) podwyzszanie B obniza wykres
    wybrani.clear();
    int i=0;
    while (wybrani.size()!=WYBRANI)
    {
        if (rand()%100<(100*pow(2.718, 0*i)))
        {
            wybrani.push_back(populacja[i]);
            //cout << i << " ";
        }
        i++;
        if (i==POCZATKOWA_POPULACJA)
            i=0;
    }
    populacja.clear();
}

void Gatunek::selekcjaRankEli()
{
    wybrani.clear();
    wybrani.push_back(populacja[0]);
    wybrani.push_back(populacja[1]);

    while (wybrani.size()!=WYBRANI)
    {
        int losowa=rand()%((POCZATKOWA_POPULACJA-1)*(POCZATKOWA_POPULACJA/2));
        int suma=0;
        for (int i=0; i<populacja.size(); i++)
        {
            suma+=(299-i);
            if (suma>losowa)
            {
                wybrani.push_back(populacja[i]);
                break;
            }
        }
    }

    populacja.clear();
}

void Gatunek::krzyzowanieOX()
{
    int potomkowieNaOsobnika=ceil(double(POCZATKOWA_POPULACJA)/double(2*WYBRANI));
    int rodzice[2][ROZMIAR_PROBLEMU];
    int dziecko[2][ROZMIAR_PROBLEMU];

    populacja.push_back(wybrani[0]); // sel elitarna
    populacja.push_back(wybrani[1]);

    for (int i=0; i<wybrani.size(); i++)
    {
        for (int a=0; a<ROZMIAR_PROBLEMU; a++)
            rodzice[0][a]=wybrani[i].geny[a];
        for (int j=0; j<potomkowieNaOsobnika; j++)
        {
            if (populacja.size()!=POCZATKOWA_POPULACJA)
            {
                vector <int> os0;
                vector <int> os1;
                int losowa=rand()%wybrani.size();
                for (int a=0; a<ROZMIAR_PROBLEMU; a++)
                {
                    rodzice[1][a]=wybrani[losowa].geny[a];
                    os0.push_back(wybrani[i].geny[a]);
                    os1.push_back(wybrani[losowa].geny[a]);
                }

                // poczatek krzyzowania
                for (int a=0; a<ROZMIAR_PROBLEMU/2; a++)
                {
                    dziecko[0][a]=rodzice[0][a];
                    dziecko[1][a]=rodzice[1][a];
                    for (int b=0; b<os1.size(); b++)
                    {
                        if (dziecko[0][a]==os1[b])
                        {
                            os1.erase(os1.begin()+b);
                            break;
                        }
                    }
                    for (int b=0; b<os0.size(); b++)
                    {
                        if (dziecko[1][a]==os0[b])
                        {
                            os0.erase(os0.begin()+b);
                            break;
                        }
                    }
                }
                for (int k=ROZMIAR_PROBLEMU/2; k<ROZMIAR_PROBLEMU; k++)
                {
                    dziecko[0][k]=os1[k-ROZMIAR_PROBLEMU/2];
                    dziecko[1][k]=os0[k-ROZMIAR_PROBLEMU/2];
                }
                populacja.push_back(dziecko[0]);
                if (populacja.size()!=POCZATKOWA_POPULACJA)
                    populacja.push_back(dziecko[1]);
            }
        }
    }
}

void Gatunek::krzyzowanieCX()
{
    int potomkowieNaOsobnika=ceil(double(POCZATKOWA_POPULACJA)/double(2*WYBRANI));
    int rodzice[2][ROZMIAR_PROBLEMU];
    int dziecko[2][ROZMIAR_PROBLEMU];
    int losowa;
    bool zakaz[ROZMIAR_PROBLEMU];
    int zakazane=0;

    populacja.push_back(wybrani[0]); // sel elitarna
    populacja.push_back(wybrani[1]);

    for (int i=0; i<wybrani.size(); i++)
    {
        for (int a=0; a<ROZMIAR_PROBLEMU; a++)
            rodzice[0][a]=wybrani[i].geny[a];
        for (int j=0; j<potomkowieNaOsobnika; j++)
        {
            if (populacja.size()!=POCZATKOWA_POPULACJA)
            {
                losowa=rand()%wybrani.size();
                for (int a=0; a<ROZMIAR_PROBLEMU; a++)
                {
                    rodzice[1][a]=wybrani[losowa].geny[a];
                    zakaz[a]=false;
                }

                //poczatek krzyzowania
                int tura=0;

                while (zakazane!=ROZMIAR_PROBLEMU)
                {
                    int k;
                    for (k=0; k<ROZMIAR_PROBLEMU; k++)
                        if (zakaz[k]==false)
                            break;


                    int poczatek=rodzice[0][k];

                    do
                    {
                        zakaz[k]=true;
                        zakazane++;
                        dziecko[tura][k]=rodzice[0][k];
                        dziecko[!tura][k]=rodzice[1][k];

                        int pom=rodzice[1][k];

                        for (k=0; k<ROZMIAR_PROBLEMU; k++)
                            if (rodzice[0][k]==pom)
                            {
                                break;
                            }

                    }   while(poczatek!=rodzice[0][k]);
                    tura=!tura;
                }
                populacja.push_back(dziecko[0]);
                if (populacja.size()!=POCZATKOWA_POPULACJA)
                    populacja.push_back(dziecko[1]);
                zakazane=0;
            }
        }
    }
}

void Gatunek::krzyzowaniePMX()
{
    int potomkowieNaOsobnika=ceil(double(POCZATKOWA_POPULACJA)/double(2*WYBRANI));
    int rodzice[2][ROZMIAR_PROBLEMU];
    int dziecko[2][ROZMIAR_PROBLEMU];
    int losowa;
    bool zakaz[2][ROZMIAR_PROBLEMU];

    populacja.push_back(wybrani[0]); // sel elitarna
    populacja.push_back(wybrani[1]);

    for (int i=0; i<wybrani.size(); i++)
    {
        for (int a=0; a<ROZMIAR_PROBLEMU; a++)
            rodzice[0][a]=wybrani[i].geny[a];
        for (int j=0; j<potomkowieNaOsobnika; j++)
        {
            if (populacja.size()!=POCZATKOWA_POPULACJA)
            {
                losowa=rand()%wybrani.size();
                for (int a=0; a<ROZMIAR_PROBLEMU; a++)
                {
                    rodzice[1][a]=wybrani[losowa].geny[a];
                    zakaz[0][a]=false;
                    zakaz[1][a]=false;
                }

                //poczatek krzyzowania

                vector <int> powtorzenia0;
                vector <int> powtorzenia1;

                for (int a=0; a<ROZMIAR_PROBLEMU/2; a++)
                {
                    dziecko[0][a]=rodzice[0][a];
                    dziecko[1][a]=rodzice[1][a];

                    bool flaga0=false;
                    bool flaga1=false;

                    for (int b=0; b<ROZMIAR_PROBLEMU/2; b++)
                    {
                        if (rodzice[0][a]==rodzice[1][b])
                            flaga0=true;
                        if (rodzice[1][a]==rodzice[0][b])
                            flaga1=true;
                    }
                    if (flaga0==false)
                        powtorzenia0.push_back(a);
                    if (flaga1==false)
                        powtorzenia1.push_back(a);
                }


                while (powtorzenia0.size()!=0)
                {
                    int a=powtorzenia0[0];
                    do
                    {
                        for (int b=0; b<ROZMIAR_PROBLEMU; b++)
                            if (rodzice[1][a]==rodzice[0][b])
                            {
                                a=b;
                                break;
                            }
                    } while (a<ROZMIAR_PROBLEMU/2);
                    dziecko[1][a]=rodzice[0][powtorzenia0[0]];
                    zakaz[1][a]=true;
                    powtorzenia0.erase(powtorzenia0.begin());



                    a=powtorzenia1[0];
                    do
                    {
                        for (int b=0; b<ROZMIAR_PROBLEMU; b++)
                            if (rodzice[0][a]==rodzice[1][b])
                            {
                                a=b;
                                break;
                            }
                    } while (a<ROZMIAR_PROBLEMU/2);
                    dziecko[0][a]=rodzice[1][powtorzenia1[0]];
                    zakaz[0][a]=true;
                    powtorzenia1.erase(powtorzenia1.begin());
                }

                for (int a=ROZMIAR_PROBLEMU/2; a<ROZMIAR_PROBLEMU; a++)
                {
                    if (zakaz[0][a]==false)
                        dziecko[0][a]=rodzice[1][a];
                    if (zakaz[1][a]==false)
                        dziecko[1][a]=rodzice[0][a];
                }

                populacja.push_back(dziecko[0]);
                if (populacja.size()!=POCZATKOWA_POPULACJA)
                    populacja.push_back(dziecko[1]);
            }
        }
    }
}


void Gatunek::ustalenieWspolczynnikaMutacji()
{
    int podobienstwoPopulacji=0;
    for (int i=0; i<POCZATKOWA_POPULACJA; i++)
    {
        for (int j=i+1; j<POCZATKOWA_POPULACJA; j++)
        {
            int podobienstwoGenow=0;
            for (int k=0; k<ROZMIAR_PROBLEMU; k++)
            {
                if (populacja[i].geny[k]==populacja[j].geny[k])
                    podobienstwoGenow++;
            }
            if (podobienstwoGenow>=progPodobienstwa)
                podobienstwoPopulacji++;
        }
    }
    szansaNaMutacje=100*podobienstwoPopulacji/(POCZATKOWA_POPULACJA*(POCZATKOWA_POPULACJA-1)/2);
    if (szansaNaMutacje>=80)
        szansaNaMutacje=80;
    //cout << szansaNaMutacje << " " << endl;
}


void Gatunek::mutacja()
{
    for (int i=2; i<populacja.size(); i++) // 2 osobniki przechodza bezposrednio
        populacja[i].mutuj();
}


void Gatunek::ocena()
{
    for (int i=0; i<populacja.size(); i++)
        populacja[i].ocena();

    Osobnik pom;  // sortowanie
    for (int i = 0; i < populacja.size(); i++)
    {
        for (int j = 0; j < populacja.size() - 1; j++)
        {
            if (populacja[j].getWynik() > populacja[j+1].getWynik())
            {
                pom=populacja[j];
                populacja[j]=populacja[j+1];
                populacja[j+1]=pom;
            }
        }
    } // end_sortowanie
}


bool Gatunek::sprawdzaniePoprawnosciPopulacji()
{
    for (int i=0; i<POCZATKOWA_POPULACJA; i++)
    {
        int suma=0;
        for (int j=0; j<ROZMIAR_PROBLEMU; j++)
            suma+=populacja[i].geny[j];
        if (suma!=(ROZMIAR_PROBLEMU*ROZMIAR_PROBLEMU/2-ROZMIAR_PROBLEMU/2))
            return false;
    }
    return true;
}


void Gatunek::rysujTabele()
{
    cout << "MIASTA - ODLEGLOSCI: " << endl;
    cout << "  ";
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
        cout << "|" << setw(2) << i;
    cout << endl;
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
    {
        cout << setw(2) << i << "|";
        for (int j=0; j<ROZMIAR_PROBLEMU; j++)
        {
            cout << setw(2) << miasta[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << "WYDARZENIA - FLOW: " << endl;
    cout << "  ";
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
        cout << "|" << setw(2) << i;
    cout << endl;
    for (int i=0; i<ROZMIAR_PROBLEMU; i++)
    {
        cout << setw(2) << i << "|";
        for (int j=0; j<ROZMIAR_PROBLEMU; j++)
        {
            cout << setw(2) << flow[i][j] << " ";
        }
        cout << endl;
    }
}

void Gatunek::zapisDoPliku()
{
    fstream plik;
    plik.open("populacja.txt", ios::out | ios::app);

    if( plik.good() == true )
    {
        for (int i=0; i<POCZATKOWA_POPULACJA; i++)
        {
            plik << setw(3) << i << ": ";
            for (int j=0; j<ROZMIAR_PROBLEMU; j++)
                plik << setw(2) << populacja[i].geny[j] << " ";
            plik << endl;
        }
        plik.close();
    }

    plik.open( "wybrani.txt", ios::out);
    if( plik.good() == true )
    {
        for (int i=0; i<WYBRANI; i++)
        {
            plik << setw(3) << i << ": ";
            for (int j=0; j<ROZMIAR_PROBLEMU; j++)
                plik << setw(2) << wybrani[i].geny[j] << " ";
            plik << endl;
        }
        plik.close();
    }

}


