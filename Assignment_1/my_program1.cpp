#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>

using namespace std;

struct element {
    int wierzcholek;
    int waga;
};

class GrafListaSasiedztwa {
    private:
        vector<list<element>> lista_sasiedztwa;
        int liczba_wierzcholkow;
        int liczba_krawedzi;
        int brak_pliku = false;

    public:
        void read_graph(string filename);
        void to_dot(string filename);
        void is_complete();
};

void GrafListaSasiedztwa::read_graph(string filename) {
    liczba_krawedzi = 0;
    ifstream odczyt(filename);
    if (odczyt.is_open()) {
        vector<string> wiersze;
        string rekord, tmp, wiersz;
        int wierzcholek_poczatkowy, wierzcholek_koncowy, waga;
        element krawedz;
        bool wierzcholek_nie_istnieje;
        while (getline(odczyt, wiersz)) {
            wiersze.clear();
            stringstream s(wiersz);
            while (getline(s, rekord, ',')) {
                wiersze.push_back(rekord);
            }
            wierzcholek_poczatkowy = stoi(wiersze[0]);
            wierzcholek_koncowy = stoi(wiersze[1]);
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
        brak_pliku = true;
    }
}

void GrafListaSasiedztwa::to_dot(string filename) {
    ofstream zapis(filename);
    zapis << "digraph {" << endl;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        for (list<element>::iterator it = ++lista_sasiedztwa[i].begin(); it != lista_sasiedztwa[i].end(); it++) {
            zapis << "	" << lista_sasiedztwa[i].front().wierzcholek << " -> ";
            zapis << it -> wierzcholek << "[label=\"" << it -> waga << "\"];" << endl;
            liczba_krawedzi++;
        }
    }
    zapis << "}" << endl;
    zapis.close();
}

void GrafListaSasiedztwa::is_complete() {
    if (brak_pliku) {
        return;
    }
    bool kompletny = false;
    liczba_wierzcholkow = lista_sasiedztwa.size() * (lista_sasiedztwa.size() - 1) / 2;
    if (liczba_krawedzi == liczba_wierzcholkow) {
        kompletny = true;
    }
    if (kompletny) {
        cout << "graph is complete" << endl;
    }
    else {
        cout << "graph is not complete" << endl;
    }
}

int main(int argc, char* argv[]) {
    GrafListaSasiedztwa gls;
    if (argc < 2) {
        cout << "./myprogram plik_wejsciowy.csv" << endl;
        return 1;
    }
    string plik_wejsciowy(argv[1]);
    gls.read_graph(plik_wejsciowy);
    size_t indeks = plik_wejsciowy.find_last_of(".");
    string plik_wyjsciowy = plik_wejsciowy.substr(0, indeks) + ".dot";
    gls.to_dot(plik_wyjsciowy);
    gls.is_complete();
}
