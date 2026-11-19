#include "cola_solicitudes.h"
#include "util.h"
#include <iostream>
#include <utility>
using namespace std;

void ColaSolicitudes::agregar(int id, int idPas, const string& o, const string& d) {
    Solicitud* nueva = new Solicitud{id, idPas, o, d, nullptr};
    if (!fin) frente = fin = nueva;
    else { fin->sig = nueva; fin = nueva; }
    n++;
}

int ColaSolicitudes::encolar(int idPas, const string& origen, const string& destino) {
    int id = proximoId++;
    agregar(id, idPas, origen, destino);
    return id;
}

void ColaSolicitudes::restaurar(int id, int idPas, const string& o, const string& d) {
    agregar(id, idPas, o, d);
    if (id >= proximoId) proximoId = id + 1;
}

bool ColaSolicitudes::atender(Solicitud& salida) {
    if (!frente) return false;
    Solicitud* t = frente;
    salida = *t;
    salida.sig = nullptr;
    frente = frente->sig;
    if (!frente) fin = nullptr;  // la cola quedó vacía: no dejar 'fin' colgando
    delete t;
    n--;
    return true;
}

bool ColaSolicitudes::contienePasajero(int idPas) const {
    for (Solicitud* s = frente; s; s = s->sig)
        if (s->idPasajero == idPas) return true;
    return false;
}

bool ColaSolicitudes::contieneCiudad(const string& nombre) const {
    string k = aMinus(nombre);
    for (Solicitud* s = frente; s; s = s->sig)
        if (aMinus(s->origen) == k || aMinus(s->destino) == k) return true;
    return false;
}

void ColaSolicitudes::listar() const {
    if (!frente) { cout << "  (la cola está vacía)\n"; return; }
    int pos = 1;
    for (Solicitud* s = frente; s; s = s->sig, pos++)
        cout << "  " << pos << ". Solicitud #" << s->id << " | pasajero " << s->idPasajero
             << " | " << s->origen << " -> " << s->destino << "\n";
}

void ColaSolicitudes::vaciar() {
    while (frente) {
        Solicitud* t = frente;
        frente = frente->sig;
        delete t;
    }
    fin = nullptr;
    n = 0;
}

void ColaSolicitudes::intercambiar(ColaSolicitudes& o) {
    swap(frente, o.frente);
    swap(fin, o.fin);
    swap(n, o.n);
    swap(proximoId, o.proximoId);
}
