#include "util.h"
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cmath>
#include <cctype>
#include <unistd.h>
using namespace std;

bool g_eof = false;

string recortar(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

string aMinus(const string& s) {
    string r = s;
    for (size_t i = 0; i < r.size(); i++) r[i] = (char)tolower((unsigned char)r[i]);
    return r;
}

bool leerLinea(const string& prompt, string& out) {
    cout << prompt;
    if (!getline(cin, out)) { g_eof = true; cout << "\n"; return false; }
    out = recortar(out);
    if (!isatty(STDIN_FILENO)) cout << out << "\n";  // eco cuando la entrada viene de un archivo (evidencia legible)
    return true;
}

bool leerEntero(const string& prompt, int& out) {
    string s;
    if (!leerLinea(prompt, s)) return false;
    char* fin = nullptr;
    errno = 0;
    long v = strtol(s.c_str(), &fin, 10);
    if (s.empty() || *fin != '\0' || errno == ERANGE || v > INT_MAX || v < INT_MIN) {
        cout << "  [!] Entrada inválida: se esperaba un número entero.\n";
        return false;
    }
    out = (int)v;
    return true;
}

bool leerDecimal(const string& prompt, double& out) {
    string s;
    if (!leerLinea(prompt, s)) return false;
    char* fin = nullptr;
    errno = 0;
    double v = strtod(s.c_str(), &fin);
    if (s.empty() || *fin != '\0' || errno == ERANGE || !isfinite(v)) {
        cout << "  [!] Entrada inválida: se esperaba un número.\n";
        return false;
    }
    out = v;
    return true;
}

bool leerTexto(const string& prompt, string& out) {
    if (!leerLinea(prompt, out)) return false;
    if (out.empty() || out.find('|') != string::npos) {
        cout << "  [!] Texto inválido: no puede estar vacío ni contener '|'.\n";
        return false;
    }
    return true;
}
