#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <climits>

using namespace std;

using Element = pair<string, int>;

class PriorityQueue {
    private:
        int LEFT(int i);
        int RIGHT(int i);
        int PARENT(int i);
        void MIN_HEAPIFY(int i);
        void MIN_HEAP_INSERT(Element key);
        void HEAP_DECREASE_KEY(int i, Element key);
        void exchange(int i, int j);
    
    public:
        void InsertElement(Element e);
        string getMinPriority();
        Element Pop();
        void BUILD_MIN_HEAP(vector<Element> A);
        vector<Element> A;
        int heap_size;
};

void PriorityQueue::InsertElement(Element e) {
    MIN_HEAP_INSERT(e);
}

string PriorityQueue::getMinPriority() {
    return A[0].first;
}

Element PriorityQueue::Pop() {
    Element root = A[0];
    A[0] = A[heap_size - 1];
    heap_size--;
    MIN_HEAPIFY(0);
    return root;
}

void PriorityQueue::BUILD_MIN_HEAP(vector<Element> A) {
    heap_size = A.size();
    for (int i = floor(heap_size / 2); i >= 1; --i) {
        MIN_HEAPIFY(i);
    }
}

int PriorityQueue::LEFT(int i) {
    return 2 * i + 1;
}

int PriorityQueue::RIGHT(int i) {
    return 2 * i + 2;
}

int PriorityQueue::PARENT(int i) {
    return (i - 1) / 2;
}

void PriorityQueue::MIN_HEAPIFY(int i) {
    int l = LEFT(i);
    int r = RIGHT(i);
    int lowest;
    if (l < heap_size && A[l].second < A[i].second) {
        lowest = l;
    }
    else {
        lowest = i;
    }
    if (r < heap_size && A[r].second < A[lowest].second) {
        lowest = r;
    }
    if (lowest != i) {
        exchange(i, lowest);
        MIN_HEAPIFY(lowest);
    }
}

void PriorityQueue::MIN_HEAP_INSERT(Element key) {
    heap_size++;
    Element E;
    A.push_back(E);
    A[heap_size - 1].second = INT_MAX;
    HEAP_DECREASE_KEY(heap_size - 1, key);
}

void PriorityQueue::HEAP_DECREASE_KEY(int i, Element key) {
    if (key.second > A[i].second) {
        cout << "error: new key is higher than current key" << endl;
        exit(-1);
    }
    A[i].first = key.first;
    A[i].second = key.second;
    while (i != 0 && A[PARENT(i)].second > A[i].second) {
        exchange(i, PARENT(i));
        i = PARENT(i);
    }
}

void PriorityQueue::exchange(int i, int j) {
    Element tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

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
        Element EXTRACT_MIN(vector<element>& Q);
        vector<element> repairQ(vector<element> Q);
        vector<list<element>> lista_sasiedztwa;
        vector<element> roznica;
        vector<Element> rezultat_kolejki;
    
    public:
        void read_graph(string filename);
        void DIJKSTRA(string s);
        int odleglosc(string w);
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

void GrafListaSasiedztwa::DIJKSTRA(string s) {
    INITIALIZE_SINGLE_SOURCE(s);
    PriorityQueue S;
    S.BUILD_MIN_HEAP(S.A);
    vector<element> Q;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        Q.push_back(lista_sasiedztwa[i].front());
    }
    while (!Q.empty()) {
        Element u = EXTRACT_MIN(Q);
        S.InsertElement(u);
        string wierzcholek = u.first;
        int indeks;
        for (int i = 0; i < lista_sasiedztwa.size(); i++) {
            if (lista_sasiedztwa[i].front().wierzcholek == wierzcholek) {
                indeks = i;
                break;
            }
        }
        list<element>::iterator it;
        for (it = ++lista_sasiedztwa[indeks].begin(); it != lista_sasiedztwa[indeks].end(); it++) {
            RELAX(wierzcholek, (*it).wierzcholek, (*it).waga);
        }
        Q = repairQ(Q); 
    }
    rezultat_kolejki = S.A;
}

int GrafListaSasiedztwa::odleglosc(string w) {
    int odleglosc;
    for (int i = 0; i < rezultat_kolejki.size(); i++) {
        if (rezultat_kolejki[i].first == w) {
            odleglosc = rezultat_kolejki[i].second;
            break;
        }
    }
    return odleglosc;
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

Element GrafListaSasiedztwa::EXTRACT_MIN(vector<element>& Q) {
    int min = Q[0].dlugosc_sciezki;
    string wierzcholek = Q[0].wierzcholek;
    int indeks = 0;
    for (int i = 0; i < Q.size(); i++) {
        if (Q[i].dlugosc_sciezki < min) {
            min = Q[i].dlugosc_sciezki;
            wierzcholek = Q[i].wierzcholek;
            indeks = i;
        }
    }
    Element element = make_pair(wierzcholek, min);
    Q[indeks] = Q[Q.size() - 1];
    Q.pop_back();
    return element;
}

vector<element> GrafListaSasiedztwa::repairQ(vector<element> Q) {
    for (int i = 0; i < Q.size(); i++) {
        for (int j = 0; j < lista_sasiedztwa.size(); j++) {
            if (Q[i].wierzcholek == lista_sasiedztwa[j].front().wierzcholek) {
                Q[i] = lista_sasiedztwa[j].front();
            }
        }
    }
    return Q;
}

int main(int argc, char* argv[]) {
    GrafListaSasiedztwa gls;
    if (argc < 5) {
        cout << "./my_program path_to_graph_file starting_vertex end_v_1 end_v_2" << endl;
        return -1;
    }
    string plik_wejsciowy(argv[1]);
    gls.read_graph(plik_wejsciowy);
    string wierzcholek_startowy = argv[2];
    gls.DIJKSTRA(wierzcholek_startowy);
    string end_v_1, end_v_2;
    end_v_1 = argv[3];
    end_v_2 = argv[4];
    int odleglosc_do_end_v_1, odleglosc_do_end_v_2;
    odleglosc_do_end_v_1 = gls.odleglosc(end_v_1);
    odleglosc_do_end_v_2 = gls.odleglosc(end_v_2);
    cout << odleglosc_do_end_v_1 << " " << odleglosc_do_end_v_2 << endl;
}