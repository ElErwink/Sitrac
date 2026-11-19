#pragma once
#include "grafo.h"
#include "lista_pasajeros.h"
#include "cola_solicitudes.h"
#include "historial.h"

// Agrupa todas las estructuras del sistema para pasarlas a los módulos sin variables globales.
struct Sistema {
    Grafo grafo;
    ListaPasajeros pasajeros;
    ColaSolicitudes cola;
    Historial historial;

    void intercambiar(Sistema& o) {
        grafo.intercambiar(o.grafo);
        pasajeros.intercambiar(o.pasajeros);
        cola.intercambiar(o.cola);
        historial.intercambiar(o.historial);
    }
};
