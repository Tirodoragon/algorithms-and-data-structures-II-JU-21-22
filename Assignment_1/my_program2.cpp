#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class GrafMacierzSasiedztwa {
    private:
        vector<vector<int>> macierz_sasiedztwa;
        vector<int> wierzcholki;
        int liczba_wierzcholkow;
        int liczba_krawedzi;
        bool brak_pliku = false;
        bool sprawdz_wierzcholek(int w);
        void dodaj_wierzcholek();
        int znajdz_indeks(int w);

    public:
        void read_graph(string filename);
        void to_dot(string filename);
        void is_complete();
};

void GrafMacierzSasiedztwa::read_graph(string filename) {
    liczba_krawedzi = 0;
    ifstream odczyt(filename);
    if (odczyt.is_open()) {
        vector<string> wiersze;
        string rekord, tmp, wiersz;
        int wierzcholek_poczatkowy, wierzcholek_koncowy, waga;
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
            if(!sprawdz_wierzcholek(wierzcholek_poczatkowy)) {
                dodaj_wierzcholek();
                wierzcholki.push_back(wierzcholek_poczatkowy);
            }
            if(!sprawdz_wierzcholek(wierzcholek_koncowy)) {
                dodaj_wierzcholek();
                wierzcholki.push_back(wierzcholek_koncowy);
            }
            macierz_sasiedztwa[znajdz_indeks(wierzcholek_poczatkowy)][znajdz_indeks(wierzcholek_koncowy)] = waga;
        }
    }
    else {
        cout << "Nie udalo sie otworzyc pliku";
        brak_pliku = true;
    }
}

void GrafMacierzSasiedztwa::to_dot(string filename) {
    ofstream zapis(filename);
    zapis << "digraph {" << endl;
    bool brak_krawedzi = false;
    for (int i = 0; i < macierz_sasiedztwa.size(); i++) {
        for (int j = 0; j < macierz_sasiedztwa[i].size(); j++) {
            if (macierz_sasiedztwa[i][j] == 0) {
            }
            else {
                zapis << "  " << wierzcholki[i] << " -> " << wierzcholki[j] << "[label=\"" << macierz_sasiedztwa[i][j] << "\"];" << endl;
            }
        }
    }
    zapis << "}" << endl;
    zapis.close();
}

void GrafMacierzSasiedztwa::is_complete() {
    if (brak_pliku) {
        return;
    }
    for (int i = 0; i < macierz_sasiedztwa.size(); i++) {
        for (int j = 0; j < macierz_sasiedztwa[i].size(); j++) {
            if (macierz_sasiedztwa[i][j] != 0) {
                liczba_krawedzi++;
            }
        }
    }
    liczba_wierzcholkow = macierz_sasiedztwa.size() * (macierz_sasiedztwa.size() - 1) / 2;
    bool kompletny = false;
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

bool GrafMacierzSasiedztwa::sprawdz_wierzcholek(int w) {
    for (int i = 0; i < wierzcholki.size(); i++) {
        if (wierzcholki[i] == w) {
            return true;
        }
    }
    return false;
}

void GrafMacierzSasiedztwa::dodaj_wierzcholek() {
    if (macierz_sasiedztwa.size() == 0) {
        vector <int> w;
        w.push_back(0);
        macierz_sasiedztwa.push_back(w);
        return;
    }
    for (int i = 0; i < macierz_sasiedztwa.size(); i++) {
        macierz_sasiedztwa[i].push_back(0);
    }
    vector<int> w;
    for (int i = 0; i < macierz_sasiedztwa[0].size(); i++) {
        w.push_back(0);
    }
    macierz_sasiedztwa.push_back(w);
}

int GrafMacierzSasiedztwa::znajdz_indeks(int w) {
    for (int i = 0; i < wierzcholki.size(); i++) {
        if (wierzcholki[i] == w) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    GrafMacierzSasiedztwa gms;
    if (argc < 2) {
        cout << "./myprogram plik_wejsciowy.csv" << endl;
        return 1;
    }
    string plik_wejsciowy(argv[1]);
    gms.read_graph(plik_wejsciowy);
    size_t indeks = plik_wejsciowy.find_last_of(".");
    string plik_wyjsciowy = plik_wejsciowy.substr(0, indeks) + ".dot";
    gms.to_dot(plik_wyjsciowy);
    gms.is_complete();
}