#include <iostream>
#include "menus.h"
#include "util.h"
using namespace std;

int main() {
    Sistema sistema;
    cout << "SITRAC - Sistema Inteligente de Transporte y Gestión de Rutas\n";
    while (!g_eof) {
        cout << "\n===== MENÚ PRINCIPAL =====\n1. Ciudades\n2. Rutas\n3. Pasajeros\n4. Cola\n"
                "5. Historial\n6. Búsquedas\n7. Reportes\n8. Guardar/cargar\n0. Salir\n";
        int op;
        if (!leerEntero("Opción: ", op)) continue;
        switch (op) {
            case 1: menuCiudades(sistema); break;
            case 2: menuRutas(sistema); break;
            case 3: menuPasajeros(sistema); break;
            case 4: menuCola(sistema); break;
            case 5: menuHistorial(sistema); break;
            case 6: menuBusquedas(sistema); break;
            case 7: menuReportes(sistema); break;
            case 8: menuArchivos(sistema); break;
            case 0: cout << "Hasta pronto.\n"; return 0;
            default: cout << "  [!] Opción inválida. Elija una opción del menú.\n";
        }
    }
    return 0;
}
