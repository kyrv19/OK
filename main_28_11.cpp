
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <limits.h>
//#include <list>

const int ZAGESZCZENIE_KROK = 30;
const int ZAGESZCZENIE_MIN = 40;    //
const float ZAG_CONST = 0.7;
const int L_SASIADOW = 20; //liczba nowych instancji (sasiadow)

//const int LICZBA_KOLOROW = 20;

using namespace std;

#pragma region LOSOWANIE

void losuj_jedynki(int **tab,int N)
{

	int licz = 0, zageszczenie = ((N*(N - 1)) / 2)*ZAG_CONST;
	while (licz < zageszczenie)
	{
		int wiersz = rand() % (N - 1);
		int kolumna = (rand() % (N - wiersz - 1)) + wiersz + 1;
		if (!tab[wiersz][kolumna])
		{
			tab[wiersz][kolumna] = 1;
			licz++;
		}
	}
	for (int i = 0; i<N; i++)
	{
		//int wiersz = rand() % (N - 1);
		for (int j = 0; j<i; j++)
		{

			tab[i][j] = tab[j][i];

		}
	}

}

void wyswietl(int **tab, int N)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << tab[i][j] << " ";
		}
		cout << endl;
	}
}


#pragma region LISTA
struct wartosc
{
	int liczba;
	wartosc *nastepna;    // wskaŸnik na nastêpny element
	wartosc();            // konstruktor
};

wartosc::wartosc()
{
	nastepna = NULL;       // konstruktor
}

struct lista
{
	wartosc *pierwsza;  // wskaŸnik na pocz¹tek listy
	int rozmiar;
	void dodaj_z_tylu(int liczba);
	void get_first();
	void wyswietl_liste();
	bool przeszukaj_liste(int kolor, int *kolory);
	lista();
};

lista::lista()
{
	pierwsza = NULL;       // konstruktor
	rozmiar = 0;
}

void lista::wyswietl_liste()
{
	wartosc *temp = pierwsza;

	while (temp)
	{
		cout << temp->liczba << " ";
		// znajdujemy wskaznik na ostatni element
		temp = temp->nastepna;
	}
	cout << endl;
}


bool lista::przeszukaj_liste(int kolor, int *kolory)
{
	wartosc *temp = pierwsza;

	while (temp)
	{
		int i = temp->liczba;
		if (kolory[i] == kolor)return false;
		// znajdujemy wskaznik na ostatni element
		temp = temp->nastepna;
	}
	return true;
}

void lista::dodaj_z_tylu(int liczba)
{
	wartosc *nowa = new wartosc;    // tworzy nowy element listy
									// wypelniamy naszymi danymi

	nowa->liczba = liczba;
	wartosc *temp = pierwsza;
	if (pierwsza == 0) // sprawdzamy czy to pierwszy element listy
	{
		pierwsza = nowa;
		pierwsza->nastepna = NULL;
	}
	else
	{
		while (temp->nastepna != NULL)
		{
			temp = temp->nastepna;

		}
		nowa->nastepna = temp->nastepna;
		temp->nastepna = nowa;

	}
	rozmiar++;
}

void lista::get_first()
{
    //kod
}

#pragma endregion

struct instancja
{
    int *pokolorowanie;
    int *kolejnosc;
    int liczba_kolorow;
    instancja *nastepna;
    instancja(int *kolejnosc_in, int *pokolorowanie_in, int kolorow_in);
};

instancja::instancja(int *kolejnosc_in, int *pokolorowanie_in, int kolorow_in)
{
    pokolorowanie = pokolorowanie_in;
    kolejnosc = kolejnosc_in;
    liczba_kolorow = kolorow_in;
    nastepna = NULL;
}

struct listaTabu    // jest to lista tablic pokolorowan
{
    instancja *pierwsza;  // wskaŸnik na pocz¹tek listy
    int rozmiar;
    void dodaj_z_tylu(int *pokolorowanie, int *kolejnosc, int liczba_kolorow);
    void get_first(instancja *zwracana_instancja);
    listaTabu();
};

listaTabu::listaTabu()
{

    pierwsza = NULL;
    rozmiar = 0;
}

void listaTabu::dodaj_z_tylu(int *pokolorowanie, int *kolejnosc, int liczba_kolorow)
{
	instancja *nowa = new instancja(kolejnosc, pokolorowanie, liczba_kolorow);    // tworzy nowy element listy
									// wypelniamy naszymi danymi
	instancja *temp = pierwsza;
	if (pierwsza == 0) // sprawdzamy czy to pierwszy element listy
	{
		pierwsza = nowa;
		pierwsza->nastepna = NULL;
	}
	else
	{
		while (temp->nastepna != NULL)
		{
			temp = temp->nastepna;

		}
		nowa->nastepna = temp->nastepna;
		temp->nastepna = nowa;

	}
	rozmiar++;
}

void listaTabu::get_first(instancja *zwracana_instancja)
{
    zwracana_instancja = pierwsza;
    pierwsza = pierwsza->nastepna;//trzeba zwolnic pamiec po wykorzystaniu tablicy!!
    //kod, zwraca wskaznik na pole "pokolorowanie" ze struktury insatncja i usuwa z listy (tylko wsk)
}


void make_list(int **macierz, lista **tab_incydencji, int N)
{
	for (int i = 0; i < N; i++)
	{
		lista *vertex = new lista;
		tab_incydencji[i] = vertex;
		for (int j = 0; j < N; j++)
		{
			if (macierz[i][j])
			{
				vertex->dodaj_z_tylu(j);
			}
		}
	}
}

void wyswietl_liste_incydencji(lista **tab_incydencji, int *kolory, int N)
{
	for (int i = 0; i < N; i++)
	{
		cout << i << "(kolor: " << kolory[i] << "): ";
		tab_incydencji[i]->wyswietl_liste();
		cout << endl;
	}
}

void zapisz_liste(int **macierz,int N)
{
	ofstream wyj;
	wyj.open("instancja.txt");
	wyj << N << " " << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (macierz[i][j] == 1)
			{
				wyj << (i + 1) << " " << (j + 1) << endl;
			}
		}
	}
	wyj.close();
}

int szukajMaxKolor(int *kolory, int N)
{
    int maks = 0;
    for(int i = 0; i < N; i++)
    {
        if(kolory[i] > maks) maks = kolory[i];
    }
    return maks;
}

int pokoloruj(lista **tab_incydencji, int *rosnace, int *wynik, int N)
{
    int maks_kolor = 0;
	for (int i = 0; i < N; i++)
	{
		int indexMax = 0;
		for (int j = 0; j < N; j++)
		{
			if (rosnace[j] != 0 && rosnace[j] > indexMax )
			{
				indexMax = j;
			}
		}
		rosnace[indexMax] = 0;
		bool szukaj = true;
		int obecnyKolor = 1;
		while (szukaj)
		{
			if (tab_incydencji[indexMax]->przeszukaj_liste(obecnyKolor, wynik))
			{
				szukaj = false;
				break;
			}
			if (szukaj)obecnyKolor++;
		}
		wynik[indexMax] = obecnyKolor;
		if (obecnyKolor>maks_kolor)maks_kolor = obecnyKolor;
	}
	return maks_kolor;
}

bool listaTabu_compare(int *tab1, int *tab2)
{
    //kod porownania
    return false;
}

void wybierzBestPermutacje(lista **tab_incydencji,int *kolejnosc, int *pokolorowanie, int N, int *kolejnosc_zwracana, int bestCandidateLiczbaKolorow)
{
    int numerWierzcholka=4;
    int bestKolor=1000;
    int bestZmiana1=0;
    int bestZmiana2=0;

    for(int i=0;i<N;i++){
        if(i!=4){
           int mix[N];
           std::copy(kolejnosc,kolejnosc + N, mix);
           int temp=mix[4];
           mix[4]=mix[i];
           mix[i]=temp;
          // for(int j=0;j<N;j++)cout << mix[j] << ", ";
           int tenKolor=pokoloruj(tab_incydencji,mix,pokolorowanie,N);
        //   cout << " = " << tenKolor << endl;
           if(tenKolor<bestKolor){
                bestKolor=tenKolor;
                bestZmiana1=i;
                bestZmiana2=4;
                bestCandidateLiczbaKolorow=tenKolor;
                std::copy(mix,mix + N, kolejnosc_zwracana);
           }
        }

    }
    cout<<bestCandidateLiczbaKolorow<<endl;
}


int tabu(lista **tab_incydencji, int *kolejnosc, int *pokolorowanie, int N, int min_kolorow)
{
    int best_liczba_kolorow = szukajMaxKolor(pokolorowanie, N);
    instancja *best = new instancja(kolejnosc, pokolorowanie, best_liczba_kolorow);  //najlepszy ogolnie
    instancja *bestCandidate = new instancja(kolejnosc, pokolorowanie, best_liczba_kolorow);  // naj kandydat do best sposrod sasiadow
    listaTabu *tabuList = new listaTabu;
    tabuList->dodaj_z_tylu(kolejnosc, pokolorowanie, best_liczba_kolorow);
    bool stopCondition = false;
    int dzialaj = 0;
    while(dzialaj < 100)
    {
        listaTabu *sasiedzi = new listaTabu;    //korzystam z listy "listaTabu" bo to lista tablic pokolorowan
        int *bestCandidateKolejnosc = new int [N];
        int bestCandidateLiczbaKolorow;
        wybierzBestPermutacje(tab_incydencji, bestCandidate->kolejnosc, bestCandidate->pokolorowanie, N, bestCandidateKolejnosc, bestCandidateLiczbaKolorow);
        bestCandidate->liczba_kolorow = bestCandidateLiczbaKolorow;
        bestCandidate->kolejnosc = bestCandidateKolejnosc;
        //sasiedzi->get_first(bestCandidate);//trzeba zwolnic pamiec po wykorzystaniu tablicy!! (patrz: get_first)
        //ktur @UP trzeba usunac strukture "wartosc" z ktorej bierzemy tablice kolorowania
        // czy jest opcja kopiowania calej partii pamieci tablicy) tak by jej nie przepisywac recznie?
        // @UPDATE tak, jest taka opcja, patrz copy();

        /*Zrobione w wybierzBestPermutacje
        for(int i = 0; i < L_SASIADOW - 1; i++)
        {
            instancja *tempCandidate;
            sasiedzi->get_first(tempCandidate);
            if(/*not in tabuList)
            {
                if(szukajMaxKolor(tempCandidate) < szukajMaxKolor(bestCandidate))
                {
                    bestCandidate = tempCandidate;
                }

            }
        }*/
        if(bestCandidate->liczba_kolorow < best->liczba_kolorow)
        {
            best = bestCandidate;
        }
        tabuList->dodaj_z_tylu(bestCandidate->pokolorowanie, bestCandidate->kolejnosc, bestCandidate->liczba_kolorow);
        dzialaj++;
        /*if(tabuList.size > maxTabuSize)
        {
            //tabuList.remove();
        }
    }
    return szukajMaxKolor(best);
}*/
    }
    min_kolorow = best->liczba_kolorow;
}


int main()
{
	srand(time(NULL));
	//    ofstream wyj;
	//    wyj.open("wyniki1.txt");
	ifstream wej;
	wej.open("dane.txt");
	int N;
	wej >> N;
	/*int  ** macierz = new int * [N]; //wszystkie elementy na 0
	for(int i=0;i<N;i++){
        macierz[i] = new int [N];
	}*/

	lista *tab_incydencji[N];
	//list<lista*> tab_incydencji[N];

	int *rosnacoWierzcholkiWgKrawedzi= new int [N];
	int *kolejnoscWierzcholkow= new int [N];

	for(int i=0; i<N; i++)
	{

	    lista *vertex = new lista;
	    tab_incydencji[i] = vertex;
	    //tab_incydencji[i].push_front(vertex);
	}
	int numer_wierzcholka;
	int sasiad;
	int p=0;
	while(!wej.eof())
	{
	    p++;
	    wej>>numer_wierzcholka;
	    numer_wierzcholka-=1;
		rosnacoWierzcholkiWgKrawedzi[numer_wierzcholka]++;
	    wej>>sasiad;
	    sasiad-=1;
		//rosnacoWierzcholkiWgKrawedzi[sasiad]++;
	    // cout << numer_wierzcholka << " XX " << sasiad << endl;
	    tab_incydencji[sasiad]->dodaj_z_tylu(numer_wierzcholka);
	    //tab_incydencji[sasiad].front().dodaj(numer);
	    //tab_incydencji[numer_wierzcholka]->dodaj(sasiad);
	}



	//losuj_jedynki(macierz);

	//zapisz_liste(macierz);
	//    mieszaj(macierz);

	//    wyswietl(macierz);
	//lista *tab_incydencji[N];//TABLICA WSKAZNIKOW NA LISTY SASIADOW KAZDEGO WIERZCHOLKA
	//make_list(macierz, tab_incydencji);
	//posortujWierzcholki(tab_incydencji, rosnacoWierzcholkiWgKrawedzi);

    int *pokolorowanie = new int[N];
	int *rosnace= new int [N];

	for (int i = 0; i < N; i++)
	{
		int indexMax = 0;
		for (int j = 0; j < N; j++)
		{
			if (rosnacoWierzcholkiWgKrawedzi[j] != 0 && rosnacoWierzcholkiWgKrawedzi[j] > indexMax )
			{
				indexMax = j;
			}
		}
		rosnace[i]=indexMax;
		rosnacoWierzcholkiWgKrawedzi[indexMax] = 0;
    }
    int best_wynik = 100;
	tabu(tab_incydencji, rosnace, pokolorowanie, N, best_wynik);
	//    wyswietl_liste_incydencji(tab_incydencji);
	cout << "MAX KOLOR: " << best_wynik << endl;



	wej.close();
	return 0;
}
