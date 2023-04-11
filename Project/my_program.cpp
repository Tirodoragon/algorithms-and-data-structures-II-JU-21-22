#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

using Element = pair<int, int>;

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
        int getMinPriority();
        Element Pop();
        void BUILD_MIN_HEAP(vector<Element> A);
        vector<Element> A;
        int heap_size;
};

void PriorityQueue::InsertElement(Element e) {
    MIN_HEAP_INSERT(e);
}

int PriorityQueue::getMinPriority() {
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
    int wierzcholek;
    int waga;
    int cena;
    int dlugosc_sciezki;
    string rodzic;
};

class GrafListaSasiedztwa {
    private:
        vector<list<element>> lista_sasiedztwa;
        void INITIALIZE_SINGLE_SOURCE(int s);
        void RELAX(int u, int v, int w);
        Element EXTRACT_MIN(vector<element>& Q);
        vector<element> repairQ(vector<element> Q);

    public:
        vector<Element> rezultat_kolejki;
        void read_graph();
        void dodaj_krawedz(int a, int b, int w);
        GrafListaSasiedztwa* odwroc();
        void DIJKSTRA(int s);
        vector<element> zwroc_ceny();
        void to_dot(string filename);
};

void GrafListaSasiedztwa::read_graph() {
   int ilosc_wierzcholkow;
    cin >> ilosc_wierzcholkow;
    for (int i = 0; i < ilosc_wierzcholkow; i++) {
        element e;
        e.wierzcholek = i + 1;
        cin >> e.cena;
        list<element> l;
        l.push_back(e);
        lista_sasiedztwa.push_back(l);
    }
    int ilosc_krawedzi;
    cin >> ilosc_krawedzi;
    for (int i = 0; i < ilosc_krawedzi; i++) {
        int wierzcholek_poczatkowy, wierzcholek_koncowy, waga;
        cin >> wierzcholek_poczatkowy >> wierzcholek_koncowy >> waga;
        dodaj_krawedz(wierzcholek_poczatkowy, wierzcholek_koncowy, waga);
    }
}

void GrafListaSasiedztwa::dodaj_krawedz(int a, int b, int w) {
    element e;
    e.wierzcholek = b;
    e.waga = w;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        if (lista_sasiedztwa[i].front().wierzcholek == a) {
            lista_sasiedztwa[i].push_back(e);
            return;
        }
    }
}

GrafListaSasiedztwa* GrafListaSasiedztwa::odwroc() {
    GrafListaSasiedztwa* gls2 = new GrafListaSasiedztwa();
    int wierzcholek_poczatkowy, wierzcholek_koncowy, waga;
    element krawedz;
    bool wierzcholek_nie_istnieje;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        for (list<element>::iterator it = ++lista_sasiedztwa[i].begin(); it != lista_sasiedztwa[i].end(); it++) {
            wierzcholek_poczatkowy = it -> wierzcholek;
            wierzcholek_koncowy = lista_sasiedztwa[i].front().wierzcholek;
            krawedz.waga = it -> waga;
            krawedz.wierzcholek = wierzcholek_koncowy;
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < gls2 -> lista_sasiedztwa.size(); i++) {
                if (gls2 -> lista_sasiedztwa[i].front().wierzcholek == wierzcholek_poczatkowy) {
                    wierzcholek_nie_istnieje = false;
                    gls2 -> lista_sasiedztwa[i].push_back(krawedz);
                    break;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_poczatkowy;
                list<element> lista;
                lista.push_back(krawedz);
                gls2 -> lista_sasiedztwa.push_back(lista);
                krawedz.wierzcholek = wierzcholek_koncowy;
                gls2 -> lista_sasiedztwa.back().push_back(krawedz);
            }
            wierzcholek_nie_istnieje = true;
            for (int i = 0; i < gls2 -> lista_sasiedztwa.size(); i++) {
                if (gls2 -> lista_sasiedztwa[i].front().wierzcholek == wierzcholek_koncowy) {
                    wierzcholek_nie_istnieje = false;
                    break;
                }
            }
            if (wierzcholek_nie_istnieje) {
                krawedz.wierzcholek = wierzcholek_koncowy;
                list<element> lista;
                lista.push_back(krawedz);
                gls2 -> lista_sasiedztwa.push_back(lista);
            }
        }
    }
    return gls2;
}

static bool sorting_func(pair<int, int>&a, pair<int, int>&b) {
    return a.second < b.second;
}

void GrafListaSasiedztwa::DIJKSTRA(int s) {
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
        int wierzcholek = u.first;
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
    sort(rezultat_kolejki.begin(), rezultat_kolejki.end(), sorting_func);
}

void GrafListaSasiedztwa::INITIALIZE_SINGLE_SOURCE(int s) {
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
    if (lista_sasiedztwa.size() != 0) {
        lista_sasiedztwa[indeks].front().dlugosc_sciezki = 0;
    }
}

void GrafListaSasiedztwa::RELAX(int u, int v, int w) {
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
    int wierzcholek = Q[0].wierzcholek;
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

vector<element> GrafListaSasiedztwa::zwroc_ceny() {
    vector<element> v;
    for (int i = 0; i < lista_sasiedztwa.size(); i++) {
        element e;
        e.wierzcholek = lista_sasiedztwa[i].front().wierzcholek;
        e.cena = lista_sasiedztwa[i].front().cena;
        v.push_back(e);
    }
    return v;
}

int znajdz_najmniejszy(vector<Element> v, vector<Element> v2, vector<element> cena) {
    vector<pair<int, int>> vp;
    for (auto& i : v) {
        vp.push_back(make_pair(i.first, i.second));
    }
    for (auto& i : v2) {
        for (int j = 0; j < vp.size(); j++) {
            if (vp[j].first == i.first && i.second == INT_MAX) {
                vp[j] = make_pair(vp[j].first, INT_MAX);
            }
            else if (vp[j].first == i.first && vp[j].second != INT_MAX) {
                vp[j] = make_pair(vp[j].first, i.second + vp[j].second);
            }
        }
    }
    for (auto& i : cena) {
        for (int j = 0; j < vp.size(); j++) {
            if (vp[j].first == i.wierzcholek && i.cena == INT_MAX) {
                vp[j] = make_pair(vp[j].first, INT_MAX);
            }
            else if (vp[j].first == i.wierzcholek && vp[j].second != INT_MAX) {
                vp[j] = make_pair(vp[j].first, i.cena / 2 + vp[j].second);
            }
        }
    }
    sort(vp.begin(), vp.end(), sorting_func);
    return vp[0].second;
}

int main() {
    GrafListaSasiedztwa gls;
    gls.read_graph();
    GrafListaSasiedztwa* gls2 = gls.odwroc();
    gls.DIJKSTRA(1);
    gls2 -> DIJKSTRA(1);
    cout << znajdz_najmniejszy(gls.rezultat_kolejki, gls2 -> rezultat_kolejki, gls.zwroc_ceny()) << endl;
}