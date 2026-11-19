#include "grafo.h"
#include "util.h"
#include <iostream>
#include <utility>
#include <algorithm>
#include <limits>
using namespace std;

Ciudad* Grafo::buscar(const string& nombre) const {
    string k = aMinus(nombre);
    for (Ciudad* c = cabeza; c; c = c->sig)
        if (aMinus(c->nombre) == k) return c;
    return nullptr;
}

bool Grafo::agregarCiudad(const string& nombre) {
    if (nombre.empty() || buscar(nombre)) return false;  // duplicada: no se crea nodo
    Ciudad* nueva = new Ciudad{nombre, nullptr, nullptr};
    if (!cabeza) cabeza = nueva;
    else {
        Ciudad* u = cabeza;
        while (u->sig) u = u->sig;
        u->sig = nueva;
    }
    n++;
    return true;
}

bool Grafo::quitarArista(Ciudad* origen, Ciudad* destino) {
    Arista* ant = nullptr;
    for (Arista* e = origen->ady; e; ant = e, e = e->sig) {
        if (e->destino == destino) {
            if (ant) ant->sig = e->sig; else origen->ady = e->sig;
            delete e;
            return true;
        }
    }
    return false;
}

bool Grafo::eliminarCiudad(const string& nombre) {
    Ciudad* c = buscar(nombre);
    if (!c) return false;
    // 1) Quitar toda arista de otras ciudades que apunte a c (evita punteros colgantes)
    for (Ciudad* x = cabeza; x; x = x->sig)
        if (x != c) quitarArista(x, c);
    // 2) Liberar las aristas propias de c
    while (c->ady) {
        Arista* t = c->ady;
        c->ady = t->sig;
        delete t;
    }
    // 3) Desenlazar y borrar el nodo
    Ciudad* ant = nullptr;
    for (Ciudad* x = cabeza; x; ant = x, x = x->sig) {
        if (x == c) {
            if (ant) ant->sig = x->sig; else cabeza = x->sig;
            break;
        }
    }
    delete c;
    n--;
    return true;
}

bool Grafo::agregarRuta(const string& a, const string& b, double dist, string& error) {
    if (dist <= 0) { error = "la distancia debe ser positiva (mayor que 0)"; return false; }
    Ciudad* ca = buscar(a);
    Ciudad* cb = buscar(b);
    if (!ca || !cb) { error = "alguna de las ciudades no existe"; return false; }
    if (ca == cb) { error = "no se permite una ruta de una ciudad a sí misma"; return false; }
    for (Arista* e = ca->ady; e; e = e->sig)
        if (e->destino == cb) { error = "esa ruta ya existe"; return false; }
    ca->ady = new Arista{cb, dist, ca->ady};
    cb->ady = new Arista{ca, dist, cb->ady};
    return true;
}

bool Grafo::eliminarRuta(const string& a, const string& b) {
    Ciudad* ca = buscar(a);
    Ciudad* cb = buscar(b);
    if (!ca || !cb) return false;
    if (!quitarArista(ca, cb)) return false;  // ruta inexistente
    quitarArista(cb, ca);
    return true;
}

bool Grafo::listarVecinos(const string& nombre) const {
    Ciudad* c = buscar(nombre);
    if (!c) return false;
    if (!c->ady) { cout << "  " << c->nombre << " no tiene conexiones.\n"; return true; }
    cout << "  Vecinos de " << c->nombre << ":\n";
    for (Arista* e = c->ady; e; e = e->sig)
        cout << "   - " << e->destino->nombre << " (" << e->dist << " km)\n";
    return true;
}

void Grafo::listarCiudades() const {
    if (!cabeza) { cout << "  (no hay ciudades registradas)\n"; return; }
    for (Ciudad* c = cabeza; c; c = c->sig) {
        int g = 0;
        for (Arista* e = c->ady; e; e = e->sig) g++;
        cout << "  - " << c->nombre << " (" << g << " conexiones)\n";
    }
}

void Grafo::listarRutas() const {
    bool hay = false;
    for (Ciudad* c = cabeza; c; c = c->sig)
        for (Arista* e = c->ady; e; e = e->sig)
            if (aMinus(c->nombre) < aMinus(e->destino->nombre)) {  // cada ruta una sola vez
                cout << "  " << c->nombre << " <-> " << e->destino->nombre << " : " << e->dist << " km\n";
                hay = true;
            }
    if (!hay) cout << "  (no hay rutas registradas)\n";
}

int Grafo::grado(const string& nombre) const {
    Ciudad* c = buscar(nombre);
    if (!c) return -1;
    int g = 0;
    for (Arista* e = c->ady; e; e = e->sig) g++;
    return g;
}

int Grafo::numRutas() const {
    int total = 0;
    for (Ciudad* c = cabeza; c; c = c->sig)
        for (Arista* e = c->ady; e; e = e->sig) total++;
    return total / 2;
}

double Grafo::distanciaMinima(const string& a, const string& b, vector<string>& camino) const {
    camino.clear();
    Ciudad* o = buscar(a);
    Ciudad* d = buscar(b);
    if (!o || !d) return -2;
    vector<Ciudad*> nodos;
    for (Ciudad* c = cabeza; c; c = c->sig) nodos.push_back(c);
    int m = (int)nodos.size();
    auto idx = [&](Ciudad* c) { for (int i = 0; i < m; i++) if (nodos[i] == c) return i; return -1; };
    const double INF = numeric_limits<double>::infinity();
    vector<double> dist(m, INF);
    vector<int> prev(m, -1);
    vector<bool> vis(m, false);
    dist[idx(o)] = 0;
    for (int it = 0; it < m; it++) {
        int u = -1;
        for (int i = 0; i < m; i++)
            if (!vis[i] && dist[i] < INF && (u == -1 || dist[i] < dist[u])) u = i;
        if (u == -1) break;
        vis[u] = true;
        for (Arista* e = nodos[u]->ady; e; e = e->sig) {
            int v = idx(e->destino);
            if (dist[u] + e->dist < dist[v]) { dist[v] = dist[u] + e->dist; prev[v] = u; }
        }
    }
    int t = idx(d);
    if (dist[t] == INF) return -1;
    for (int x = t; x != -1; x = prev[x]) camino.push_back(nodos[x]->nombre);
    reverse(camino.begin(), camino.end());
    return dist[t];
}

void Grafo::vaciar() {
    while (cabeza) {
        Ciudad* c = cabeza;
        cabeza = c->sig;
        while (c->ady) { Arista* t = c->ady; c->ady = t->sig; delete t; }
        delete c;
    }
    n = 0;
}

void Grafo::intercambiar(Grafo& o) {
    swap(cabeza, o.cabeza);
    swap(n, o.n);
}
