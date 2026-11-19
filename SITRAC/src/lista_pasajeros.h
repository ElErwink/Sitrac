#pragma once
#include <string>

// Lista simplemente enlazada PROPIA de pasajeros (sin std::list).
struct Pasajero {
    int id;
    std::string nombre;
    int edad;
    Pasajero* sig;
};

class ListaPasajeros {
    Pasajero* cabeza;
    int n;
public:
    ListaPasajeros() : cabeza(nullptr), n(0) {}
    ~ListaPasajeros() { vaciar(); }
    ListaPasajeros(const ListaPasajeros&) = delete;
    ListaPasajeros& operator=(const ListaPasajeros&) = delete;

    bool insertar(int id, const std::string& nombre, int edad);  // false si el ID ya existe
    Pasajero* buscar(int id) const;                              // nullptr si no existe
    bool actualizar(int id, const std::string& nombre, int edad);
    bool eliminar(int id);                                       // false si no existe
    void listar() const;
    void vaciar();
    int tamano() const { return n; }
    const Pasajero* primero() const { return cabeza; }
    void intercambiar(ListaPasajeros& o);
};
