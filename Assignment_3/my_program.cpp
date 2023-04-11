#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct element {
    string wierzcholek;
    int waga;
    string kolor;
    string rodzic;
    int czas_szaro;
    int czas_czarno;
};

class GrafListaSasiedztwa {
    private:
        vector<list<element>> lista_sasiedztwa;
        int czas;

    public:
        void read_graph(string filename);
        GrafListaSasiedztwa* DFS();
        void DFSVISIT(string w);
        list<string> sortowanie;
        int suma(string s);
};

void GrafListaSasiedztwa::read_graph(string filename) {
    ifstream odczyt(filename);
    if (odczyt.is_open()) {
        vector<string> wiersze;
        string rekord, tmp, wiersz;
        string wierzcholek_poczatkowy, wierzcholek_koncowy;
        int waga;
        element krawedz;
        bool wierzcholek_nie_istnieje;
        while (getline(odczyt, wiersz)) {
            wiersze.clear();
            stringstream s(wiersz);
            while (getline(s, rekord, ',')) {
                wiersze.push_back(rekord);
            }
            wierzcholek_poczatkowy = wiersze[0];
            wierzcholek_koncowy = wiersze[1];
            waga = stoi(wiersze[2]);
            krawedz.wierzcholek = wierzcholek_koncowy;
            krawedz.waga = waga;
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < lista_sasiedztwa.size(); i++) {
                if (lista_sasiedztwa[i].front().wierzcholek == wierzcholek_poczatkowy) {
                    wierzcholek_nie_istnieje = false;
                    lista_sasiedztwa[i].push_back(krawedz);
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_poczatkowy;
                krawedz.waga = 0;
                list<element> lista;
                lista.push_back(krawedz);
                lista_sasiedztwa.push_back(lista);
                krawedz.wierzcholek = wierzcholek_koncowy;
                krawedz.waga = waga;
                lista_sasiedztwa.back().push_back(krawedz);
            }
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < lista_sasiedztwa.size(); i++) {
                if (lista_sasiedztwa[i].front().wierzcholek == wierzcholek_koncowy) {
                    wierzcholek_nie_istnieje = false;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_koncowy;
                krawedz.waga = 0;
                list<element> lista;
                lista.push_back(krawedz);
                lista_sasiedztwa.push_back(lista);
            }
        }
        odczyt.close();
    }
    else {
        cout << "Nie udalo sie otworzyc pliku";
    }
}

GrafListaSasiedztwa* GrafListaSasiedztwa::DFS() {
    GrafListaSasiedztwa* gls2 = new GrafListaSasiedztwa();
    element krawedz;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        krawedz.wierzcholek = lista_sasiedztwa[i].front().wierzcholek;
        krawedz.waga = lista_sasiedztwa[i].front().waga;
        krawedz.kolor = "b";
        krawedz.rodzic = "null";
        list<element> lista;
        lista.push_back(krawedz);
        for (list<element>::iterator it = ++lista_sasiedztwa[i].begin(); it != lista_sasiedztwa[i].end(); it++) {
            lista.push_back(*it);
        }
        vector<list<element>>::iterator it2;
        for (it2 = gls2 -> lista_sasiedztwa.begin(); it2 != gls2 -> lista_sasiedztwa.end(); it2++) {
            if (suma((*it2).front().wierzcholek) > suma(lista_sasiedztwa[i].front().wierzcholek)) {
                    gls2 -> lista_sasiedztwa.insert(it2, lista);
                    break;
            }
        }
        if (it2 == gls2 -> lista_sasiedztwa.end()) {
            gls2 -> lista_sasiedztwa.push_back(lista);
        }
    }
    gls2 -> czas = 0;
    vector<list<element>>::iterator it2;
    for (int i = 0; i < gls2 -> lista_sasiedztwa.size(); i++) {
        if (gls2 -> lista_sasiedztwa[i].front().kolor == "b") {
            gls2 -> DFSVISIT(gls2 -> lista_sasiedztwa[i].front().wierzcholek);
        }
    }
    return gls2;
}

void GrafListaSasiedztwa::DFSVISIT(string w) {
    int indeks;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        if (lista_sasiedztwa[i].front().wierzcholek == w) {
            indeks = i;
        }
    }
    lista_sasiedztwa[indeks].front().czas_szaro = czas;
    lista_sasiedztwa[indeks].front().kolor = "s";
    for (list<element>::iterator it = ++lista_sasiedztwa[indeks].begin(); it != lista_sasiedztwa[indeks].end(); it++) {
        for (int i = 0; i < lista_sasiedztwa.size(); i++) {
            if (lista_sasiedztwa[i].front().wierzcholek == (*it).wierzcholek && lista_sasiedztwa[i].front().kolor == "b") {
                lista_sasiedztwa[i].front().rodzic = w;
                DFSVISIT((*it).wierzcholek);
            }
        }
    }
    lista_sasiedztwa[indeks].front().kolor = "c";
    czas++;
    lista_sasiedztwa[indeks].front().czas_czarno = czas;
    sortowanie.push_front(w);
}

int GrafListaSasiedztwa::suma (string s){
    int suma = 0;
    for (int i = 0; i < s.size(); i++){
        suma += pow(10, (s.size()-i))*s[i];
    }
    return suma;
}

int main(int argc, char* argv[]) {
    GrafListaSasiedztwa gls1;
    string plik_wejsciowy(argv[1]);
    gls1.read_graph(plik_wejsciowy);
    GrafListaSasiedztwa* gls2 = gls1.DFS();
    list<string>::iterator it;
    for (it = gls2 -> sortowanie.begin(); it != gls2 -> sortowanie.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;
}