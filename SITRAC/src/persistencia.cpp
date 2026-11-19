#include "persistencia.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

static vector<string> partir(const string& linea) {
    vector<string> campos;
    string actual;
    for (char c : linea) {
        if (c == '|') { campos.push_back(actual); actual.clear(); }
        else actual += c;
    }
    campos.push_back(actual);
    return campos;
}

bool guardarSistema(const string& archivo, const Sistema& s, string& error) {
    ofstream out(archivo);
    if (!out) { error = "no se pudo abrir el archivo para escribir"; return false; }
    out << setprecision(12);
    out << "SITRAC|1\n";
    for (const Ciudad* c = s.grafo.primera(); c; c = c->sig) out << "C|" << c->nombre << "\n";
    for (const Ciudad* c = s.grafo.primera(); c; c = c->sig)
        for (const Arista* e = c->ady; e; e = e->sig)
            if (c->nombre < e->destino->nombre)
                out << "R|" << c->nombre << "|" << e->destino->nombre << "|" << e->dist << "\n";
    for (const Pasajero* p = s.pasajeros.primero(); p; p = p->sig)
        out << "P|" << p->id << "|" << p->nombre << "|" << p->edad << "\n";
    for (const Solicitud* q = s.cola.verFrente(); q; q = q->sig)
        out << "Q|" << q->id << "|" << q->idPasajero << "|" << q->origen << "|" << q->destino << "\n";
    // El historial se guarda de más antiguo a más reciente para reconstruir la pila al cargar
    vector<string> lineas;
    for (const Registro* r = s.historial.ultimo(); r; r = r->sig) {
        ostringstream l;
        l << setprecision(12) << "H|" << r->idSolicitud << "|" << r->idPasajero << "|"
          << r->origen << "|" << r->destino << "|" << r->distancia;
        lineas.push_back(l.str());
    }
    for (int i = (int)lineas.size() - 1; i >= 0; i--) out << lineas[i] << "\n";
    out << "N|" << s.cola.siguienteId() << "\n";
    if (!out) { error = "error al escribir el archivo"; return false; }
    return true;
}

bool cargarSistema(const string& archivo, Sistema& s, string& error) {
    ifstream in(archivo);
    if (!in) { error = "el archivo no existe o no se puede abrir"; return false; }
    Sistema temp;
    string linea;
    int nro = 0;
    bool cabecera = false;
    while (getline(in, linea)) {
        nro++;
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        if (linea.empty()) continue;
        vector<string> f = partir(linea);
        ostringstream msg;
        msg << "línea " << nro << " inválida";
        try {
            if (f[0] == "SITRAC" && f.size() == 2) { cabecera = true; continue; }
            if (!cabecera) { error = "el archivo no es un respaldo de SITRAC"; return false; }
            if (f[0] == "C" && f.size() == 2) {
                if (!temp.grafo.agregarCiudad(f[1])) { error = msg.str() + " (ciudad repetida)"; return false; }
            } else if (f[0] == "R" && f.size() == 4) {
                string e;
                if (!temp.grafo.agregarRuta(f[1], f[2], stod(f[3]), e)) { error = msg.str() + " (" + e + ")"; return false; }
            } else if (f[0] == "P" && f.size() == 4) {
                if (!temp.pasajeros.insertar(stoi(f[1]), f[2], stoi(f[3]))) { error = msg.str() + " (ID duplicado)"; return false; }
            } else if (f[0] == "Q" && f.size() == 5) {
                temp.cola.restaurar(stoi(f[1]), stoi(f[2]), f[3], f[4]);
            } else if (f[0] == "H" && f.size() == 6) {
                temp.historial.apilar(stoi(f[1]), stoi(f[2]), f[3], f[4], stod(f[5]));
            } else if (f[0] == "N" && f.size() == 2) {
                temp.cola.fijarSiguienteId(stoi(f[1]));
            } else {
                error = msg.str();
                return false;
            }
        } catch (...) {
            error = msg.str() + " (número mal formado)";
            return false;
        }
    }
    if (!cabecera) { error = "el archivo está vacío o no es un respaldo de SITRAC"; return false; }
    s.intercambiar(temp);  // temp (con los datos viejos) se destruye al salir
    return true;
}
