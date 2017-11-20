
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <limits.h>

int N = 7; // wymiar macierzy (liczba wierzcholkow
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

void wyswietl(int **tab)
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
	void dodaj(int liczba);
	void wyswietl_liste();
	bool przeszukaj_liste(int kolor, int *kolory);
	lista();
};

lista::lista()
{
	pierwsza = NULL;       // konstruktor
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

void lista::dodaj(int liczba)
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
}

#pragma endregion

struct instancja
{
    int *pokolorowanie;
    instancja *next;
    instancja();
};

instancja::instancja()
{
    next = NULL;
}

struct listaTabu    // jest to lista tablic pokolorowan
{
    instancja *pierwsza;  // wskaŸnik na pocz¹tek listy
    void dodaj(int *pokolorowanie);
    void get_first(int *kolorwanie);
    listaTabu();
};

listaTabu::listaTabu()
{
    pierwsza = NULL;
}

void listaTabu::dodaj(int *pokolorowanie)
{
    //kod dodania do listy
}

void listaTabu::get_first(int *kolorwanie)
{
    kolorwanie = pierwsza->pokolorowanie;
    pierwsza = pierwsza->next;//trzeba zwolnic pamiec po wykorzystaniu tablicy!!
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
				vertex->dodaj(j);
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

void pokoloruj(lista **tab_incydencji, int *rosnace, int *wynik, int N)
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
}

bool listaTabu_compare(int *tab1, int *tab2)
{
    //kod porownania
    return false;
}

void znajdz_sasiadow(lista **tab_incydencji, int *kolory, listaTabu *sasiedzi)
{
    int sasiadow_do_znalezienia = L_SASIADOW;
    //kod
}

/*
int tabu(lista **tab_incydencji)
{
    int best[N];
    int bestCandidate[N];   // naj kandydat do best sposrod sasiadow
    int best_liczba_kolorow;
    pokoloruj(tab_incydencji, best);
    listaTabu *tabuList = new listaTabu;
    tabuList->dodaj(best);
    bool stopCondition = false;
    while(!stopCondition)
    {
        listaTabu *sasiedzi = new listaTabu;    //korzystam z listy "listaTabu" bo to lista tablic pokolorowan
        znajdz_sasiadow(tab_incydencji, bestCandidate, sasiedzi);
        sasiedzi->get_first(bestCandidate);//trzeba zwolnic pamiec po wykorzystaniu tablicy!! (patrz: get_first)
        //ktur @UP trzeba usunac strukture "wartosc" z ktorej bierzemy tablice kolorowania
        // czy jest opcja kopiowania calej partii pamieci tablicy) tak by jej nie przepisywac recznie?
        // @UPDATE tak, jest taka opcja, patrz copy();
        for(int i = 0; i < L_SASIADOW - 1; i++)
        {
            int tempCandidate[N];
            sasiedzi->get_first(tempCandidate);
            if(/*not in tabuList &&*/
               //szukajMaxKolor(tempCandidate) < szukajMaxKolor(bestCandidate))

           // {
                //copy()
          //  }
      //  }/
        /*
  	for (sCandidate in sNeighborhood)
  		if ( (not tabuList.contains(sCandidate)) and (fitness(sCandidate) > fitness(bestCandidate)) )
 			bestCandidate ← sCandidate
 		end
 	end
	if (fitness(bestCandidate) > fitness(sBest))
		sBest ← bestCandidate
 	end
 	tabuList.push(bestCandidate)
 	if (tabuList.size > maxTabuSize)
 		tabuList.removeFirst()
	stopCond = checkStopCond(stopCond)

    }
    best_liczba_kolorow = szukajMaxKolor(best);
    return best_liczba_kolorow;
}
*/

int main()
{
	srand(time(NULL));
	//    ofstream wyj;
	//    wyj.open("wyniki1.txt");
	ifstream wej;
	wej.open("queen6.txt");

	wej >> N;
	int  ** macierz = new int * [N]; //wszystkie elementy na 0
	for(int i=0;i<N;i++){
        macierz[i] = new int [N];
	}
	lista *tab_incydencji[N];
	int *rosnacoWierzcholkiWgKrawedzi= new int [N];
	int *kolejnoscWierzcholkow= new int [N];

	for(int i=0; i<N; i++)
	{

	    lista *vertex = new lista;
	    tab_incydencji[i] = vertex;
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
	    tab_incydencji[sasiad]->dodaj(numer_wierzcholka);
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
	pokoloruj(tab_incydencji, rosnacoWierzcholkiWgKrawedzi, pokolorowanie, N);
	//    wyswietl_liste_incydencji(tab_incydencji);
	cout << "MAX KOLOR: " << szukajMaxKolor(pokolorowanie, N) << endl;



	wej.close();
	return 0;
}
