#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <climits>

using namespace std;

struct element {
    string wierzcholek;
    int waga;
    int dlugosc_sciezki;
    string rodzic;
};

class GrafListaSasiedztwa {
    private:
        void INITIALIZE_SINGLE_SOURCE(string s);
        void RELAX(string u, string v, int w);
        vector<list<element>> lista_sasiedztwa;
    
    public:
        void read_graph(string filename);
        bool BELLMAN_FORD(string s);
        void to_map();
        map<string, int> dlugosc_sciezki;
        map<string, string> poprzednik;
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
                    break;
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
                    break;
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
        cout << "Nie udalo sie otworzyc pliku" << endl;
    }
}

bool GrafListaSasiedztwa::BELLMAN_FORD(string s) {
    INITIALIZE_SINGLE_SOURCE(s);
    int ilosc_wierzcholkow = lista_sasiedztwa.size() - 1;
    string wierzcholek_poczatkowy, wierzcholek_koncowy;
    int waga;
    for (int i = 1; i < ilosc_wierzcholkow; i++) {
        list<element>::iterator it;
        for (int j = 0; j < lista_sasiedztwa.size(); j++) {
            wierzcholek_poczatkowy = lista_sasiedztwa[j].front().wierzcholek;
            for (it = ++lista_sasiedztwa[j].begin(); it != lista_sasiedztwa[j].end(); it++) {
                wierzcholek_koncowy = (*it).wierzcholek;
                waga = (*it).waga;
                RELAX(wierzcholek_poczatkowy, wierzcholek_koncowy, waga);
            }
        }
    }
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        list<element>::iterator it2;
        for (it2 = ++lista_sasiedztwa[i].begin(); it2 != lista_sasiedztwa[i].end(); it2++) {
            for (int j = 0; j < lista_sasiedztwa.size(); j++) {
                if (lista_sasiedztwa[j].front().wierzcholek == (*it2).wierzcholek) {
                    if (lista_sasiedztwa[j].front().dlugosc_sciezki > lista_sasiedztwa[i].front().dlugosc_sciezki + (*it2).waga 
                        && lista_sasiedztwa[i].front().dlugosc_sciezki != INT_MAX) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void GrafListaSasiedztwa::INITIALIZE_SINGLE_SOURCE(string s) {
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        lista_sasiedztwa[i].front().dlugosc_sciezki = INT_MAX;
        lista_sasiedztwa[i].front().rodzic = "NIL";
    }
    int indeks;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        if (lista_sasiedztwa[i].front().wierzcholek == s) {
            indeks = i;
            break;
        }
    }
    lista_sasiedztwa[indeks].front().dlugosc_sciezki = 0;
}

void GrafListaSasiedztwa::RELAX(string u, string v, int w) {
    int indeks_u, indeks_v;
    bool znalezione_u = false, znalezione_v = false;
    for (int i = 0; i < lista_sasiedztwa.size() && !(znalezione_u && znalezione_v); i++) {
        if (lista_sasiedztwa[i].front().wierzcholek == u) {
            indeks_u = i;
            znalezione_u = true;
        }
        if (lista_sasiedztwa[i].front().wierzcholek == v) {
            indeks_v = i;
            znalezione_v = true;
        }
    }
    list<element>::iterator it;
    if (lista_sasiedztwa[indeks_v].front().dlugosc_sciezki > lista_sasiedztwa[indeks_u].front().dlugosc_sciezki + w
        && lista_sasiedztwa[indeks_u].front().dlugosc_sciezki != INT_MAX) {
        lista_sasiedztwa[indeks_v].front().dlugosc_sciezki = lista_sasiedztwa[indeks_u].front().dlugosc_sciezki + w;
        lista_sasiedztwa[indeks_v].front().rodzic = u;
    }
}

void GrafListaSasiedztwa::to_map() {
    string etykieta_wierzcholka, rodzic;
    int dlugosc;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        etykieta_wierzcholka = lista_sasiedztwa[i].front().wierzcholek;
        dlugosc = lista_sasiedztwa[i].front().dlugosc_sciezki;
        dlugosc_sciezki[etykieta_wierzcholka] = dlugosc;
        rodzic = lista_sasiedztwa[i].front().rodzic;
        poprzednik[etykieta_wierzcholka] = rodzic;
    }
}

int main(int argc, char* argv[]) {
    GrafListaSasiedztwa gls;
    if (argc < 3) {
        cout << "./my_program path_to_graph_file starting_vertex" << endl;
        return -1;
    }
    string plik_wejsciowy(argv[1]);
    gls.read_graph(plik_wejsciowy);
    string wierzcholek_startowy = argv[2];
    bool sukces;
    sukces = gls.BELLMAN_FORD(wierzcholek_startowy);
    cout << (sukces ? "TRUE" : "FALSE") << endl;
    gls.to_map();
    for (const auto & elem : gls.dlugosc_sciezki) {
        cout << "(" << elem.first << "," << (elem.second == INT_MAX ? "INF" : to_string(elem.second)) << "), ";
    }
    cout << endl;
    for (const auto & elem : gls.poprzednik) {
        cout << "(" << elem.first << "," << (elem.second == "NIL" ? "NULL" : elem.second) << "), ";
    }
    cout << endl;
}