#include "menus.h"
#include "util.h"
#include "persistencia.h"
#include <iostream>
#include <vector>
using namespace std;

static const char* ARCHIVO_DEFECTO = "sitrac_datos.txt";
static void opcionInvalida() { cout << "  [!] Opción inválida. Elija una opción del menú.\n"; }

// ---------------------------------------------------------------- CIUDADES
void menuCiudades(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== CIUDADES ===\n1. Agregar ciudad\n2. Eliminar ciudad\n3. Listar ciudades\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        string nombre;
        switch (op) {
            case 1:
                if (!leerTexto("Nombre de la ciudad: ", nombre)) break;
                if (s.grafo.agregarCiudad(nombre)) cout << "  [OK] Ciudad '" << nombre << "' registrada.\n";
                else cout << "  [!] La ciudad '" << nombre << "' ya existe. No se creó ningún nodo nuevo.\n";
                break;
            case 2:
                if (!leerTexto("Ciudad a eliminar: ", nombre)) break;
                if (!s.grafo.existeCiudad(nombre)) { cout << "  [!] La ciudad no existe.\n"; break; }
                if (s.cola.contieneCiudad(nombre)) {
                    cout << "  [!] No se puede eliminar: hay solicitudes pendientes en la cola que usan esa ciudad.\n";
                    break;
                }
                {
                    int g = s.grafo.grado(nombre);
                    s.grafo.eliminarCiudad(nombre);
                    cout << "  [OK] Ciudad eliminada junto con sus " << g << " conexiones.\n";
                }
                break;
            case 3: s.grafo.listarCiudades(); break;
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- RUTAS
void menuRutas(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== RUTAS ===\n1. Agregar ruta\n2. Eliminar ruta\n3. Consultar vecinos de una ciudad\n"
                "4. Listar todas las rutas\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        string a, b, err;
        double d;
        switch (op) {
            case 1:
                if (!leerTexto("Ciudad A: ", a) || !leerTexto("Ciudad B: ", b)) break;
                if (!leerDecimal("Distancia (km): ", d)) break;
                if (s.grafo.agregarRuta(a, b, d, err)) cout << "  [OK] Ruta " << a << " <-> " << b << " (" << d << " km) creada.\n";
                else cout << "  [!] No se creó la ruta: " << err << ".\n";
                break;
            case 2:
                if (!leerTexto("Ciudad A: ", a) || !leerTexto("Ciudad B: ", b)) break;
                if (s.grafo.eliminarRuta(a, b)) cout << "  [OK] Ruta eliminada.\n";
                else cout << "  [!] Esa ruta no existe.\n";
                break;
            case 3:
                if (!leerTexto("Ciudad: ", a)) break;
                if (!s.grafo.listarVecinos(a)) cout << "  [!] La ciudad no existe.\n";
                break;
            case 4: s.grafo.listarRutas(); break;
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- PASAJEROS
static bool leerDatosPasajero(string& nombre, int& edad) {
    if (!leerTexto("Nombre: ", nombre)) return false;
    if (!leerEntero("Edad: ", edad)) return false;
    if (edad < 0 || edad > 120) { cout << "  [!] Edad fuera de rango (0-120).\n"; return false; }
    return true;
}

void menuPasajeros(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== PASAJEROS ===\n1. Insertar\n2. Buscar por ID\n3. Actualizar\n4. Eliminar\n5. Listar\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        int id, edad;
        string nombre;
        switch (op) {
            case 1:
                if (!leerEntero("ID (entero positivo): ", id)) break;
                if (id <= 0) { cout << "  [!] El ID debe ser positivo.\n"; break; }
                if (!leerDatosPasajero(nombre, edad)) break;
                if (s.pasajeros.insertar(id, nombre, edad)) cout << "  [OK] Pasajero registrado.\n";
                else cout << "  [!] Ya existe un pasajero con el ID " << id << ". No se insertó.\n";
                break;
            case 2: {
                if (!leerEntero("ID: ", id)) break;
                Pasajero* p = s.pasajeros.buscar(id);
                if (p) cout << "  ID " << p->id << " | " << p->nombre << " | " << p->edad << " años\n";
                else cout << "  [!] No existe un pasajero con ese ID.\n";
                break;
            }
            case 3:
                if (!leerEntero("ID a actualizar: ", id)) break;
                if (!s.pasajeros.buscar(id)) { cout << "  [!] No existe un pasajero con ese ID.\n"; break; }
                if (!leerDatosPasajero(nombre, edad)) break;
                s.pasajeros.actualizar(id, nombre, edad);
                cout << "  [OK] Pasajero actualizado.\n";
                break;
            case 4:
                if (!leerEntero("ID a eliminar: ", id)) break;
                if (s.cola.contienePasajero(id)) { cout << "  [!] No se puede eliminar: tiene solicitudes pendientes en la cola.\n"; break; }
                if (s.pasajeros.eliminar(id)) cout << "  [OK] Pasajero eliminado.\n";
                else cout << "  [!] No existe un pasajero con ese ID. La lista no cambió.\n";
                break;
            case 5: s.pasajeros.listar(); break;
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- COLA
void menuCola(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== COLA DE SOLICITUDES ===\n1. Encolar solicitud de viaje\n2. Atender (desencolar)\n"
                "3. Mostrar frente\n4. Listar cola\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        switch (op) {
            case 1: {
                int id;
                string o, d;
                if (!leerEntero("ID del pasajero: ", id)) break;
                if (!s.pasajeros.buscar(id)) { cout << "  [!] El pasajero no existe. Regístrelo primero.\n"; break; }
                if (!leerTexto("Ciudad origen: ", o) || !leerTexto("Ciudad destino: ", d)) break;
                if (!s.grafo.existeCiudad(o) || !s.grafo.existeCiudad(d)) { cout << "  [!] Origen o destino no existe.\n"; break; }
                if (aMinus(o) == aMinus(d)) { cout << "  [!] Origen y destino no pueden ser iguales.\n"; break; }
                int sid = s.cola.encolar(id, o, d);
                cout << "  [OK] Solicitud #" << sid << " encolada. Pendientes: " << s.cola.tamano() << ".\n";
                break;
            }
            case 2: {
                Solicitud sol;
                if (!s.cola.atender(sol)) { cout << "  [!] La cola está vacía: no hay solicitudes por atender.\n"; break; }
                vector<string> camino;
                double dist = s.grafo.distanciaMinima(sol.origen, sol.destino, camino);
                s.historial.apilar(sol.id, sol.idPasajero, sol.origen, sol.destino, dist >= 0 ? dist : -1);
                cout << "  [OK] Atendida solicitud #" << sol.id << " (pasajero " << sol.idPasajero << "): "
                     << sol.origen << " -> " << sol.destino << "\n";
                if (dist < 0) cout << "       No existe ruta entre las ciudades.\n";
                else {
                    cout << "       Ruta más corta (" << dist << " km): ";
                    for (size_t i = 0; i < camino.size(); i++) cout << (i ? " -> " : "") << camino[i];
                    cout << "\n";
                }
                break;
            }
            case 3: {
                const Solicitud* f = s.cola.verFrente();
                if (!f) cout << "  [!] La cola está vacía.\n";
                else cout << "  Frente: solicitud #" << f->id << " | pasajero " << f->idPasajero << " | "
                          << f->origen << " -> " << f->destino << "\n";
                break;
            }
            case 4: s.cola.listar(); break;
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- HISTORIAL
void menuHistorial(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== HISTORIAL ===\n1. Listar atendidas (más reciente primero)\n2. Ver la última atendida\n"
                "3. Limpiar historial\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        switch (op) {
            case 1: s.historial.listar(); break;
            case 2: {
                const Registro* r = s.historial.ultimo();
                if (!r) cout << "  [!] El historial está vacío.\n";
                else cout << "  Última: solicitud #" << r->idSolicitud << " | " << r->origen << " -> " << r->destino << "\n";
                break;
            }
            case 3: s.historial.vaciar(); cout << "  [OK] Historial limpiado.\n"; break;
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- BÚSQUEDAS
void menuBusquedas(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== BÚSQUEDAS ===\n1. Pasajero por ID\n2. Pasajeros por nombre (contiene)\n"
                "3. Ruta más corta entre dos ciudades\n4. ¿Existe una ciudad?\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        string a, b;
        switch (op) {
            case 1: {
                int id;
                if (!leerEntero("ID: ", id)) break;
                Pasajero* p = s.pasajeros.buscar(id);
                if (p) cout << "  Encontrado: " << p->nombre << ", " << p->edad << " años\n";
                else cout << "  [!] No existe un pasajero con ese ID.\n";
                break;
            }
            case 2: {
                if (!leerTexto("Texto a buscar: ", a)) break;
                int hallados = 0;
                for (const Pasajero* p = s.pasajeros.primero(); p; p = p->sig)
                    if (aMinus(p->nombre).find(aMinus(a)) != string::npos) {
                        cout << "  ID " << p->id << " | " << p->nombre << " | " << p->edad << " años\n";
                        hallados++;
                    }
                if (!hallados) cout << "  [!] Sin coincidencias.\n";
                break;
            }
            case 3: {
                if (!leerTexto("Origen: ", a) || !leerTexto("Destino: ", b)) break;
                vector<string> camino;
                double d = s.grafo.distanciaMinima(a, b, camino);
                if (d == -2) cout << "  [!] Alguna de las ciudades no existe.\n";
                else if (d < 0) cout << "  [!] No existe ruta entre " << a << " y " << b << ".\n";
                else {
                    cout << "  Distancia mínima: " << d << " km\n  Camino: ";
                    for (size_t i = 0; i < camino.size(); i++) cout << (i ? " -> " : "") << camino[i];
                    cout << "\n";
                }
                break;
            }
            case 4:
                if (!leerTexto("Ciudad: ", a)) break;
                cout << (s.grafo.existeCiudad(a) ? "  Sí existe.\n" : "  No existe.\n");
                break;
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- REPORTES
void menuReportes(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== REPORTES ===\n1. Resumen general\n2. Ciudad con más conexiones\n"
                "3. Ciudades aisladas (sin rutas)\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        switch (op) {
            case 1:
                cout << "  Ciudades: " << s.grafo.numCiudades() << "\n  Rutas: " << s.grafo.numRutas()
                     << "\n  Pasajeros: " << s.pasajeros.tamano() << "\n  Solicitudes pendientes: " << s.cola.tamano()
                     << "\n  Solicitudes atendidas: " << s.historial.tamano() << "\n";
                break;
            case 2: {
                const Ciudad* mejor = nullptr;
                int gm = -1;
                for (const Ciudad* c = s.grafo.primera(); c; c = c->sig) {
                    int g = s.grafo.grado(c->nombre);
                    if (g > gm) { gm = g; mejor = c; }
                }
                if (!mejor) cout << "  [!] No hay ciudades registradas.\n";
                else cout << "  " << mejor->nombre << " con " << gm << " conexiones.\n";
                break;
            }
            case 3: {
                int k = 0;
                for (const Ciudad* c = s.grafo.primera(); c; c = c->sig)
                    if (!c->ady) { cout << "  - " << c->nombre << "\n"; k++; }
                if (!k) cout << "  No hay ciudades aisladas.\n";
                break;
            }
            case 0: return;
            default: opcionInvalida();
        }
    }
}

// ---------------------------------------------------------------- ARCHIVOS
void menuArchivos(Sistema& s) {
    while (!g_eof) {
        cout << "\n=== GUARDAR / CARGAR ===\n1. Guardar datos\n2. Cargar datos\n0. Volver\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        if (op == 0) return;
        if (op != 1 && op != 2) { opcionInvalida(); continue; }
        string nombre, err;
        if (!leerLinea(string("Archivo (Enter = ") + ARCHIVO_DEFECTO + "): ", nombre)) continue;
        if (nombre.empty()) nombre = ARCHIVO_DEFECTO;
        if (op == 1) {
            if (guardarSistema(nombre, s, err)) cout << "  [OK] Datos guardados en '" << nombre << "'.\n";
            else cout << "  [!] No se guardó: " << err << ".\n";
        } else {
            if (cargarSistema(nombre, s, err)) cout << "  [OK] Datos cargados desde '" << nombre << "'.\n";
            else cout << "  [!] No se cargó (los datos actuales no cambiaron): " << err << ".\n";
        }
    }
}
