#pragma once
#include <string>
#include "sistema.h"

bool guardarSistema(const std::string& archivo, const Sistema& s, std::string& error);
// Carga a una copia temporal; solo reemplaza los datos actuales si TODO el archivo es válido.
bool cargarSistema(const std::string& archivo, Sistema& s, std::string& error);
