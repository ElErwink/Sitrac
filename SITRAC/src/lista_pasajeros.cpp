#include "lista_pasajeros.h"
#include <iostream>
#include <utility>
using namespace std;

bool ListaPasajeros::insertar(int id, const string& nombre, int edad) {
    if (buscar(id)) return false;  // ID duplicado
    Pasajero* nuevo = new Pasajero{id, nombre, edad, nullptr};
    if (!cabeza) cabeza = nuevo;
    else {
        Pasajero* u = cabeza;
        while (u->sig) u = u->sig;
        u->sig = nuevo;
    }
    n++;
    return true;
}

Pasajero* ListaPasajeros::buscar(int id) const {
    for (Pasajero* p = cabeza; p; p = p->sig)
        if (p->id == id) return p;
    return nullptr;
}

bool ListaPasajeros::actualizar(int id, const string& nombre, int edad) {
    Pasajero* p = buscar(id);
    if (!p) return false;
    p->nombre = nombre;
    p->edad = edad;
    return true;
}

bool ListaPasajeros::eliminar(int id) {
    Pasajero* ant = nullptr;
    for (Pasajero* p = cabeza; p; ant = p, p = p->sig) {
        if (p->id == id) {
            if (ant) ant->sig = p->sig; else cabeza = p->sig;
            delete p;
            n--;
            return true;
        }
    }
    return false;
}

void ListaPasajeros::listar() const {
    if (!cabeza) { cout << "  (no hay pasajeros registrados)\n"; return; }
    for (Pasajero* p = cabeza; p; p = p->sig)
        cout << "  ID " << p->id << " | " << p->nombre << " | " << p->edad << " años\n";
}

void ListaPasajeros::vaciar() {
    while (cabeza) {
        Pasajero* t = cabeza;
        cabeza = cabeza->sig;
        delete t;
    }
    n = 0;
}

void ListaPasajeros::intercambiar(ListaPasajeros& o) {
    swap(cabeza, o.cabeza);
    swap(n, o.n);
}
