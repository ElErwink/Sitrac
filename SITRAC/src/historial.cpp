#include "historial.h"
#include <iostream>
#include <utility>
using namespace std;

void Historial::apilar(int idSol, int idPas, const string& o, const string& d, double dist) {
    tope = new Registro{idSol, idPas, o, d, dist, tope};
    n++;
}

void Historial::listar() const {
    if (!tope) { cout << "  (el historial está vacío)\n"; return; }
    for (Registro* r = tope; r; r = r->sig) {
        cout << "  Solicitud #" << r->idSolicitud << " | pasajero " << r->idPasajero << " | "
             << r->origen << " -> " << r->destino << " | ";
        if (r->distancia < 0) cout << "sin ruta disponible\n";
        else cout << r->distancia << " km\n";
    }
}

void Historial::vaciar() {
    while (tope) {
        Registro* t = tope;
        tope = tope->sig;
        delete t;
    }
    n = 0;
}

void Historial::intercambiar(Historial& o) {
    swap(tope, o.tope);
    swap(n, o.n);
}
