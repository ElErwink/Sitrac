#pragma once
#include <string>

// Pila enlazada PROPIA con las solicitudes ya atendidas (la más reciente arriba).
struct Registro {
    int idSolicitud;
    int idPasajero;
    std::string origen;
    std::string destino;
    double distancia;  // -1 si no había ruta
    Registro* sig;
};

class Historial {
    Registro* tope;
    int n;
public:
    Historial() : tope(nullptr), n(0) {}
    ~Historial() { vaciar(); }
    Historial(const Historial&) = delete;
    Historial& operator=(const Historial&) = delete;

    void apilar(int idSol, int idPas, const std::string& o, const std::string& d, double dist);
    const Registro* ultimo() const { return tope; }
    void listar() const;
    void vaciar();
    int tamano() const { return n; }
    void intercambiar(Historial& o);
};
