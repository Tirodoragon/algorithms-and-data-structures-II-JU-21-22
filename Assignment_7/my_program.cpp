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
};

class GrafMacierzSasiedztwa {
    private:
        vector<vector<int>> macierz_sasiedztwa;
        vector<vector<int>> floyd_warshall;
        bool sprawdz_wierzcholek(string w);
        void dodaj_wierzcholek();
        int znajdz_indeks(string w);

    public:
        vector<string> wierzcholki;
        void read_graph(string filename);
        void FLOYD_WARSHALL();
        int zwroc_z_floyda(string a, string b);
};

void GrafMacierzSasiedztwa::read_graph(string filename) {
    ifstream odczyt(filename);
    if (odczyt.is_open()) {
        vector<string> wiersze;
        string rekord, tmp, wiersz;
        string wierzcholek_poczatkowy, wierzcholek_koncowy;
        int waga;
        while (getline(odczyt, wiersz)) {
            wiersze.clear();
            stringstream s(wiersz);
            while (getline(s, rekord, ',')) {
                wiersze.push_back(rekord);
            }
            wierzcholek_poczatkowy = wiersze[0];
            wierzcholek_koncowy = wiersze[1];
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
        FLOYD_WARSHALL();
    }
    else {
        cout << "Nie udalo sie otworzyc pliku";
    }
}

void GrafMacierzSasiedztwa::FLOYD_WARSHALL() {
    vector<vector<int>> macierz_fw = vector<vector<int>> (macierz_sasiedztwa.size(), vector<int> (macierz_sasiedztwa.size(), INT32_MAX));
    for (int i = 0; i < macierz_fw.size(); i++) {
        macierz_fw[i][i] = 0;
    }
    for (int i = 0; i < macierz_fw.size(); i++) {
        for (int j = 0; j < macierz_fw.size(); j++) {
            if (macierz_sasiedztwa[i][j] != INT32_MIN) {
                macierz_fw[i][j] = macierz_sasiedztwa[i][j];
            }
        }
    }

    for (int k = 0; k < macierz_fw.size(); k++) {
        for (int i = 0; i < macierz_fw.size(); i++) {
            for (int j = 0; j < macierz_fw.size(); j++) {
                if (macierz_fw[i][j] > macierz_fw[i][k] + macierz_fw[k][j] && macierz_fw[i][k] != INT32_MAX && macierz_fw[k][j] != INT32_MAX) {
                    macierz_fw[i][j] = macierz_fw[i][k] + macierz_fw[k][j];
                }
            }
        }
    }
    
    
    floyd_warshall = macierz_fw;
}

bool GrafMacierzSasiedztwa::sprawdz_wierzcholek(string w) {
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
        macierz_sasiedztwa[i].push_back(INT32_MIN);
    }
    vector<int> w = vector<int> (macierz_sasiedztwa[0].size(), INT32_MIN);
    macierz_sasiedztwa.push_back(w);
}

int GrafMacierzSasiedztwa::znajdz_indeks(string w) {
    for (int i = 0; i < wierzcholki.size(); i++) {
        if (wierzcholki[i] == w) {
            return i;
        }
    }
    return -1;
}

int GrafMacierzSasiedztwa::zwroc_z_floyda(string a, string b) {
    return floyd_warshall[znajdz_indeks(a)][znajdz_indeks(b)];
}

int main(int argc, char* argv[]) {
    GrafMacierzSasiedztwa gms;
    if (argc < 4) {
        cout << "./my_program path_to_graph_file \'starting_vertex,exnding_vertex\' \'starting_vertex,exnding_vertex\'" << endl;
        return -1;
    }
    string plik_wejsciowy(argv[1]);
    gms.read_graph(plik_wejsciowy);
    
    for (int i = 2; i < argc; i++){
        string para = argv[i];
        string v1;
        string v2;
        bool jest_pierwszy = false;
        for (int j = 0; j < para.length(); j++){
            if (para[j] == ','){
                jest_pierwszy = true;
                continue;
            }

            if (!jest_pierwszy){
                v1 += para[j];
            }
            else{
                v2 += para[j];
            }
        }
        cout << gms.zwroc_z_floyda(v1, v2) << " ";
    }
    cout << endl;
}