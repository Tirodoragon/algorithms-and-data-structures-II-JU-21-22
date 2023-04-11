#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>

using namespace std;

struct element {
    string wierzcholek;
    string kolor;
    string rodzic;
    int czas_szaro;
    int czas_czarno;
};

class GrafListaSasiedztwa {
    private:
        int suma(string s);
        vector<list<element>> lista_sasiedztwa;
        int czas;
        list<string> sortowanie;
        list<string> kolejnosc;

    public:
        void read_graph(string filename);
        GrafListaSasiedztwa* DFS();
        void DFSVISIT(string w, bool b = false);
        GrafListaSasiedztwa* odwroc();
        GrafListaSasiedztwa* DFS_odwrocony();
};

void GrafListaSasiedztwa::read_graph(string filename) {
    ifstream odczyt(filename);
    if (odczyt.is_open()) {
        string wyraz;
        string wierzcholek_poczatkowy, wierzcholek_koncowy;
        element krawedz;
        bool wierzcholek_nie_istnieje;
        while(odczyt >> wyraz) {
            if (wyraz == "digraph") {
                continue;
            }
            if (wyraz == "{") {
                continue;
            }
            if (wyraz == "}") {
                break;
            }
            wierzcholek_poczatkowy = wyraz;
            odczyt >> wierzcholek_koncowy;
            odczyt >> wierzcholek_koncowy;
            wierzcholek_koncowy.pop_back();
            krawedz.wierzcholek = wierzcholek_koncowy;
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < lista_sasiedztwa.size(); i++) {
                if (lista_sasiedztwa[i].front().wierzcholek == wierzcholek_poczatkowy) {
                    wierzcholek_nie_istnieje = false;
                    lista_sasiedztwa[i].push_back(krawedz);
                    break;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_poczatkowy;
                list<element> lista;
                lista.push_back(krawedz);
                lista_sasiedztwa.push_back(lista);
                krawedz.wierzcholek = wierzcholek_koncowy;
                lista_sasiedztwa.back().push_back(krawedz);
            }
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < lista_sasiedztwa.size(); i++) {
                if (lista_sasiedztwa[i].front().wierzcholek == wierzcholek_koncowy) {
                    wierzcholek_nie_istnieje = false;
                    break;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_koncowy;
                list<element> lista;
                lista.push_back(krawedz);
                lista_sasiedztwa.push_back(lista);
            }
        }
        odczyt.close();
    }
    else {
        cout << "Nie udalo sie otworzyc pliku" << endl;
    }
}

GrafListaSasiedztwa* GrafListaSasiedztwa::DFS() {
    GrafListaSasiedztwa* gls2 = new GrafListaSasiedztwa();
    element krawedz;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        krawedz.wierzcholek = lista_sasiedztwa[i].front().wierzcholek;
        krawedz.kolor = "b";
        krawedz.rodzic = "NIL";
        list<element> lista;
        lista.push_back(krawedz);
        for (list<element>::iterator it = ++lista_sasiedztwa[i].begin(); it != lista_sasiedztwa[i].end(); it++) {
            lista.push_back(*it);
        }
        gls2 -> lista_sasiedztwa.push_back(lista);
    }
    gls2 -> czas = 0;
    vector<list<element>>::iterator it2;
    for (int i = 0; i < gls2 -> lista_sasiedztwa.size(); i++) {
        if (gls2 -> lista_sasiedztwa[i].front().kolor == "b") {
            gls2 -> DFSVISIT(gls2 -> lista_sasiedztwa[i].front().wierzcholek);
        }
    }
    gls2 -> kolejnosc = gls2 -> sortowanie;
    return gls2;
}

void GrafListaSasiedztwa::DFSVISIT(string w, bool b) {
    int indeks;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        if (lista_sasiedztwa[i].front().wierzcholek == w) {
            indeks = i;
            break;
        }
    }
    lista_sasiedztwa[indeks].front().czas_szaro = czas;
    lista_sasiedztwa[indeks].front().kolor = "s";
    for (list<element>::iterator it = ++lista_sasiedztwa[indeks].begin(); it != lista_sasiedztwa[indeks].end(); it++) {
        for (int i = 0; i < lista_sasiedztwa.size(); i++) {
            if (lista_sasiedztwa[i].front().wierzcholek == (*it).wierzcholek && lista_sasiedztwa[i].front().kolor == "b") {
                lista_sasiedztwa[i].front().rodzic = w;
                //if (b) {
                    //cout << lista_sasiedztwa[i].front().wierzcholek; wypisywanie wierzcholkow z kazdego drzewa w lesie przeszukiwania
                    // z kroku 3
                //    DFSVISIT((*it).wierzcholek, true);
                //    break;
                //}
                DFSVISIT((*it).wierzcholek);
            }
        }
    }
    lista_sasiedztwa[indeks].front().kolor = "c";
    czas++;
    lista_sasiedztwa[indeks].front().czas_czarno = czas;
    sortowanie.push_front(w);
}

GrafListaSasiedztwa* GrafListaSasiedztwa::odwroc() {
    GrafListaSasiedztwa* gls3 = new GrafListaSasiedztwa();
    string wierzcholek_poczatkowy, wierzcholek_koncowy;
    int czas_czarno;
    element krawedz;
    bool wierzcholek_nie_istnieje;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        for (list<element>::iterator it = ++lista_sasiedztwa[i].begin(); it != lista_sasiedztwa[i].end(); it++) {
            wierzcholek_poczatkowy = it -> wierzcholek;
            wierzcholek_koncowy = lista_sasiedztwa[i].front().wierzcholek;
            czas_czarno = lista_sasiedztwa[i].front().czas_czarno;
            krawedz.wierzcholek = wierzcholek_koncowy;
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < gls3 -> lista_sasiedztwa.size(); i++) {
                if (gls3 -> lista_sasiedztwa[i].front().wierzcholek == wierzcholek_poczatkowy) {
                    wierzcholek_nie_istnieje = false;
                    gls3 -> lista_sasiedztwa[i].push_back(krawedz);
                    break;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_poczatkowy;
                krawedz.czas_czarno = czas_czarno;
                list<element> lista;
                lista.push_back(krawedz);
                gls3 -> lista_sasiedztwa.push_back(lista);
                krawedz.wierzcholek = wierzcholek_koncowy;
                gls3 -> lista_sasiedztwa.back().push_back(krawedz);
            }
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < gls3 -> lista_sasiedztwa.size(); i++) {
                if (gls3 -> lista_sasiedztwa[i].front().wierzcholek == wierzcholek_koncowy) {
                    wierzcholek_nie_istnieje = false;
                    break;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_koncowy;
                krawedz.czas_czarno = it -> czas_czarno;
                list<element> lista;
                lista.push_back(krawedz);
                gls3 -> lista_sasiedztwa.push_back(lista);
            }
        }
    }
    gls3 -> kolejnosc = kolejnosc;
    return gls3;
}

GrafListaSasiedztwa* GrafListaSasiedztwa::DFS_odwrocony() {
    GrafListaSasiedztwa* gls4 = new GrafListaSasiedztwa();
    element krawedz;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        krawedz.wierzcholek = lista_sasiedztwa[i].front().wierzcholek;
        krawedz.kolor = "b";
        krawedz.rodzic = "NIL";
        list<element> lista;
        lista.push_back(krawedz);
        for (list<element>::iterator it = ++lista_sasiedztwa[i].begin(); it != lista_sasiedztwa[i].end(); it++) {
            lista.push_back(*it);
        }
        gls4 -> lista_sasiedztwa.push_back(lista);
    }
    gls4 -> czas = 0;
    vector<list<element>>::iterator it2;
    int rozmiar = kolejnosc.size();
    int licznik_spojnosci = 0;
    for (int i = 0; i < rozmiar; i++) {
        for (int j = 0; j < gls4 -> lista_sasiedztwa.size(); j++) {
            if (gls4 -> lista_sasiedztwa[j].front().wierzcholek == kolejnosc.front()) {
                if (gls4 -> lista_sasiedztwa[j].front().kolor == "b") {
                    //cout << endl << lista_sasiedztwa[j].front().wierzcholek;
                    gls4 -> DFSVISIT(kolejnosc.front(), true);
                    licznik_spojnosci++;
                }
                kolejnosc.pop_front();
                break;
            }
        }
    }
    cout << licznik_spojnosci << endl;
    return gls4;
}

int GrafListaSasiedztwa::suma(string s) {
    int suma = 0;
    for (int i = 0; i < s.size(); i++) {
        suma += pow(10, (s.size() -1)) * s[i];
    }
    return suma;
}

int main(int argc, char* argv[]) {
    GrafListaSasiedztwa gls1;
    if (argc < 2) {
        cout << "./my_program path_to_graph_file" << endl;
        return -1;
    }
    string plik_wejsciowy(argv[1]);
    gls1.read_graph(plik_wejsciowy);
    GrafListaSasiedztwa* gls2 = gls1.DFS();
    GrafListaSasiedztwa* gls3 = gls2 -> odwroc();
    GrafListaSasiedztwa* gls4 = gls3 -> DFS_odwrocony();
}