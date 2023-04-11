#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>

using namespace std;

struct element {
    string wierzcholek;
    int waga;
    string rodzic;
    int glebokosc;
};

class GrafListaSasiedztwa {
    private:
        vector<list<element>> lista_sasiedztwa;

    public:
        void read_graph(string filename);
        GrafListaSasiedztwa* BFS(string poczatek);
        list<string> get_path(string koniec);
        int get_hops(string koniec);
        bool contains(list<string> l, string s);
        string korzen;
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
            krawedz.wierzcholek = wierzcholek_poczatkowy;
            krawedz.waga = waga;
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < lista_sasiedztwa.size(); i++) {
                if (lista_sasiedztwa[i].front().wierzcholek == wierzcholek_koncowy) {
                    wierzcholek_nie_istnieje = false;
                    lista_sasiedztwa[i].push_back(krawedz);
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_koncowy;
                krawedz.waga = 0;
                list<element> lista;
                lista.push_back(krawedz);
                lista_sasiedztwa.push_back(lista);
                krawedz.wierzcholek = wierzcholek_poczatkowy;
                krawedz.waga = waga;
                lista_sasiedztwa[lista_sasiedztwa.size()-1].push_back(krawedz);
            }
        }
        odczyt.close();
    }
    else {
        cout << "Nie udalo sie otworzyc pliku";
    }
}

GrafListaSasiedztwa* GrafListaSasiedztwa::BFS(string poczatek) {
    GrafListaSasiedztwa* gls2 = new GrafListaSasiedztwa();
    element krawedz;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        if (lista_sasiedztwa[i].front().wierzcholek == poczatek) {
            krawedz.wierzcholek = lista_sasiedztwa[i].front().wierzcholek;
            krawedz.waga = lista_sasiedztwa[i].front().waga;
            list<element> lista;
            krawedz.glebokosc = 0;
            krawedz.rodzic = "null";
            lista.push_back(krawedz);
            gls2 -> lista_sasiedztwa.push_back(lista);
        }
        else {
            krawedz.wierzcholek = lista_sasiedztwa[i].front().wierzcholek;
            krawedz.waga = lista_sasiedztwa[i].front().waga;
            list<element> lista;
            lista.push_back(krawedz);
            gls2 -> lista_sasiedztwa.push_back(lista);
        }
    }
    list<string> kolejka;
    kolejka.push_back(poczatek);
    string u;
    int indeks;
    int glebokosc_rodzica;
    list<string> odwiedzone;
    odwiedzone.push_back(poczatek);
    while (!kolejka.empty()) {
        u = kolejka.front();
        kolejka.pop_front();
        for (int i = 0; i < gls2 -> lista_sasiedztwa.size(); i++) {
            if (gls2 -> lista_sasiedztwa[i].front().wierzcholek == u) {
                indeks = i;
            }
        }
        glebokosc_rodzica = gls2 -> lista_sasiedztwa[indeks].front().glebokosc;
        for (list<element>::iterator it = ++lista_sasiedztwa[indeks].begin(); it != lista_sasiedztwa[indeks].end(); it++) {
            if (!contains(odwiedzone, (*it).wierzcholek)) {
                (*it).glebokosc = glebokosc_rodzica + 1;
                (*it).rodzic = u;
                for (int j = 0; j < gls2 -> lista_sasiedztwa.size(); j++){
                    if (gls2 -> lista_sasiedztwa[j].front().wierzcholek == (*it).wierzcholek){
                        gls2 -> lista_sasiedztwa[j].front().rodzic = u;
                        gls2 -> lista_sasiedztwa[j].front().glebokosc = glebokosc_rodzica + 1;
                        break;
                    }
                }
                kolejka.push_back((*it).wierzcholek);
                odwiedzone.push_back((*it).wierzcholek);
                gls2 -> lista_sasiedztwa[indeks].push_back((*it));
            }
        }
    }
    return gls2;
}

list<string> GrafListaSasiedztwa::get_path(string koniec) {
    list<string> sciezka;
    string rodzic = koniec;
    while (rodzic != "null"){
        int i;
        for (i = 0; i < lista_sasiedztwa.size(); i++){
            if (lista_sasiedztwa[i].front().wierzcholek == rodzic){
                rodzic = lista_sasiedztwa[i].front().rodzic;
                sciezka.push_front(lista_sasiedztwa[i].front().wierzcholek);
                break;
            }
        }
        if (i == lista_sasiedztwa.size()){
            return list<string>();
        }
    }
    return sciezka;
}

int GrafListaSasiedztwa::get_hops(string koniec) {
    for (int i = 0; i < lista_sasiedztwa.size(); i++){
        if (lista_sasiedztwa[i].front().wierzcholek == koniec){
            return lista_sasiedztwa[i].front().glebokosc;
        }
    }
    return -1;
}

bool GrafListaSasiedztwa::contains(list<string> l, string s){
    for (list<string>::iterator it = l.begin(); it != l.end(); it++){
        if ((*it) == s){
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    GrafListaSasiedztwa gls1;
    string plik_wejsciowy(argv[1]);
    gls1.read_graph(plik_wejsciowy);
    string poczatek = argv[2];
    GrafListaSasiedztwa* gls2 = gls1.BFS(poczatek);
    for (int i = 0; i < argc - 3; i++) {
        cout << gls2 -> get_hops(argv[i + 3]) << " ";
    }
    cout << endl;
}