#pragma once
#include <string>

// Entrada robusta: ninguna función lanza excepciones ni queda en ciclo infinito.
extern bool g_eof;  // true si se acabó la entrada (Ctrl+D / fin de archivo)

std::string recortar(const std::string& s);
std::string aMinus(const std::string& s);
bool leerLinea(const std::string& prompt, std::string& out);
bool leerEntero(const std::string& prompt, int& out);
bool leerDecimal(const std::string& prompt, double& out);
bool leerTexto(const std::string& prompt, std::string& out);  // no vacío y sin '|'
