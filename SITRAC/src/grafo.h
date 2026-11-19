#pragma once
#include <string>
#include <vector>

// Grafo NO dirigido y ponderado con LISTA DE ADYACENCIA PROPIA.
// Las ciudades forman una lista enlazada; cada ciudad tiene su lista enlazada de aristas.
struct Ciudad;
struct Arista {
    Ciudad* destino;
    double dist;
    Arista* sig;
};
struct Ciudad {
    std::string nombre;
    Arista* ady;
    Ciudad* sig;
};

class Grafo {
    Ciudad* cabeza;
    int n;
    Ciudad* buscar(const std::string& nombre) const;
    static bool quitarArista(Ciudad* origen, Ciudad* destino);
public:
    Grafo() : cabeza(nullptr), n(0) {}
    ~Grafo() { vaciar(); }
    Grafo(const Grafo&) = delete;
    Grafo& operator=(const Grafo&) = delete;

    bool agregarCiudad(const std::string& nombre);       // false si ya existe
    bool existeCiudad(const std::string& nombre) const { return buscar(nombre) != nullptr; }
    bool eliminarCiudad(const std::string& nombre);      // borra también sus conexiones (sin referencias colgantes)
    bool agregarRuta(const std::string& a, const std::string& b, double dist, std::string& error);
    bool eliminarRuta(const std::string& a, const std::string& b);
    bool listarVecinos(const std::string& nombre) const; // false si la ciudad no existe
    void listarCiudades() const;
    void listarRutas() const;
    int grado(const std::string& nombre) const;          // -1 si no existe
    int numCiudades() const { return n; }
    int numRutas() const;
    // Dijkstra. Retorna distancia mínima; -1 = no hay ruta; -2 = alguna ciudad no existe.
    double distanciaMinima(const std::string& a, const std::string& b, std::vector<std::string>& camino) const;
    const Ciudad* primera() const { return cabeza; }
    void vaciar();
    void intercambiar(Grafo& o);
};
