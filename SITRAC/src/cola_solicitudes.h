#pragma once
#include <string>

// Cola FIFO enlazada PROPIA de solicitudes de viaje (sin std::queue).
struct Solicitud {
    int id;
    int idPasajero;
    std::string origen;
    std::string destino;
    Solicitud* sig;
};

class ColaSolicitudes {
    Solicitud* frente;
    Solicitud* fin;
    int n;
    int proximoId;
    void agregar(int id, int idPas, const std::string& o, const std::string& d);
public:
    ColaSolicitudes() : frente(nullptr), fin(nullptr), n(0), proximoId(1) {}
    ~ColaSolicitudes() { vaciar(); }
    ColaSolicitudes(const ColaSolicitudes&) = delete;
    ColaSolicitudes& operator=(const ColaSolicitudes&) = delete;

    int encolar(int idPas, const std::string& origen, const std::string& destino);  // retorna el ID asignado
    void restaurar(int id, int idPas, const std::string& o, const std::string& d);  // usado al cargar archivo
    bool atender(Solicitud& salida);        // false si la cola está vacía (subdesbordamiento controlado)
    const Solicitud* verFrente() const { return frente; }
    bool contienePasajero(int idPas) const;
    bool contieneCiudad(const std::string& nombre) const;
    void listar() const;
    void vaciar();
    int tamano() const { return n; }
    int siguienteId() const { return proximoId; }
    void fijarSiguienteId(int v) { if (v > proximoId) proximoId = v; }
    void intercambiar(ColaSolicitudes& o);
};
