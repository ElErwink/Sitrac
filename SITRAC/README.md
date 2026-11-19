# SITRAC — Sistema Inteligente de Transporte y Gestión de Rutas

Estructura de Datos · Universidad Santo Tomás · Proyecto integrador 2026-II
Avance de la sesión del 12 de noviembre de 2026 (versión formativa, no final).

**Integrantes:** _(completar: nombre 1 — nombre 2)_

## Compilar y ejecutar

```bash
make            # genera el ejecutable ./sitrac  (g++ con C++17)
./sitrac
make clean
```

Sin `make`: `g++ -std=c++17 -Wall -Wextra src/*.cpp -o sitrac`

## Ejecutar las pruebas

```bash
bash pruebas/ejecutar_pruebas.sh     # regenera los registros en evidencia/
```

## Diagrama modular

```
                          main.cpp  (solo el menú principal)
                              │
                          menus.cpp  ── un submenú por módulo
      ┌──────┬──────┬────────┼────────┬──────────┬──────────┬──────────┐
  Ciudades Rutas Pasajeros   Cola   Historial  Búsquedas  Reportes  Archivos
      │      │      │          │        │          │          │          │
      └──┬───┘      │          │        │          │          │          │
         ▼          ▼          ▼        ▼          │          │          ▼
     grafo.cpp  lista_pasajeros  cola_solicitudes  historial  │    persistencia.cpp
   (lista de     .cpp (lista      .cpp (cola       .cpp (pila   │    (guardar/cargar
   adyacencia)   enlazada)        enlazada)        enlazada)    │     archivo de texto)
         ▲                                                       │
         └───────────── Dijkstra / consultas ────────────────────┘

   sistema.h agrupa las 4 estructuras en un struct Sistema que se pasa por referencia
   (no hay variables globales de datos). util.cpp: lectura segura de entradas.
```

## Tabla de estructuras

| Operación | Estructura usada | Justificación |
|---|---|---|
| Registrar, buscar, actualizar, eliminar y listar pasajeros | **Lista simplemente enlazada propia** (`ListaPasajeros`) | Se insertan y eliminan pasajeros con frecuencia y no se conoce la cantidad: no hay que reservar ni desplazar memoria. Búsqueda O(n) aceptable para este volumen. |
| Ciudades y rutas (alta/baja, vecinos) | **Grafo no dirigido con lista de adyacencia propia** (`Grafo`) | Las rutas son dispersas (pocas conexiones por ciudad): la lista de adyacencia usa O(V+E) memoria frente a O(V²) de una matriz, y consultar vecinos es recorrer una lista. |
| Solicitudes de viaje | **Cola enlazada propia con punteros `frente` y `fin`** (`ColaSolicitudes`) | Las solicitudes se atienden por orden de llegada (FIFO). Con `fin` el encolar es O(1) y atender es O(1). |
| Historial de atendidas | **Pila enlazada propia** (`Historial`) | Interesa lo más reciente primero (LIFO); apilar y consultar el último es O(1). |
| Ruta más corta | **Dijkstra** sobre el grafo | Las distancias son positivas, que es la condición para Dijkstra. |
| Guardar/cargar | Archivo de texto con `\|` como separador | Formato legible y fácil de depurar; la carga se valida completa antes de reemplazar los datos actuales. |

Se usa `std::string` y `std::vector` solo como apoyo (textos y arreglos auxiliares dentro de Dijkstra y la persistencia). Las estructuras centrales **no** usan `std::list` ni `std::queue`.

## Casos de prueba

Entradas en `pruebas/*.in`, registros de ejecución en `evidencia/*.txt`.

| Caso | Acción | Resultado obtenido | Evidencia |
|---|---|---|---|
| CP-01 | Registrar Bogotá dos veces | Rechaza el duplicado; solo queda 1 nodo | `evidencia/CP-01_ciudad_duplicada.txt` |
| CP-02 | Eliminar pasajero inexistente | Informa y la lista no cambia | `evidencia/CP-02_eliminar_pasajero_inexistente.txt` |
| CP-03 | Atender cola vacía | Mensaje de cola vacía, sin romper | `evidencia/CP-03_atender_cola_vacia.txt` |
| CP-04 | Ruta con distancia -5 | Rechaza la distancia inválida | `evidencia/CP-04_ruta_distancia_negativa.txt` |
| CP-05 | Eliminar ciudad conectada | Se borran sus aristas en ambos sentidos; los vecinos quedan sin referencias a ella | `evidencia/CP-05_eliminar_ciudad_conectada.txt` |
| CP-06 | Letras donde se pide número | Mensaje de error y el menú sigue; sin ciclo infinito | `evidencia/CP-06_letras_en_vez_de_numero.txt` |
| Extra 07 | Flujo completo (ciudades, rutas, pasajeros, cola, historial, guardado) | Correcto; incluye ID duplicado y pasajero inexistente | `evidencia/EXTRA-07_flujo_completo.txt` |
| Extra 08 | Guardar, cargar y cargar archivo inexistente | Restaura datos; el archivo inexistente no altera nada | `evidencia/EXTRA-08_guardar_y_cargar.txt` |

También se compiló con `-fsanitize=address,undefined` y los 8 casos terminan sin errores de memoria ni fugas.

## Funcionalidades

| Funcionalidad | Estado | Responsable |
|---|---|---|
| Menú principal persistente + 8 submenús con Volver | Terminado | _(completar)_ |
| Lista enlazada de pasajeros (insertar, buscar, actualizar, eliminar, listar) | Terminado | _(completar)_ |
| Grafo: alta ciudad, alta/baja de conexiones, vecinos, eliminar ciudad sin referencias colgantes | Terminado | _(completar)_ |
| Cola de solicitudes (encolar, atender, mostrar frente) | Terminado | _(completar)_ |
| Historial (pila) | Terminado | _(completar)_ |
| Búsquedas (pasajero por ID/nombre, ruta más corta) | Terminado | _(completar)_ |
| Reportes básicos | Terminado | _(completar)_ |
| Guardar/cargar en archivo de texto | Terminado | _(completar)_ |
| Pruebas con entradas inválidas | Terminado | _(completar)_ |
| **Pendiente:** pruebas unitarias automáticas con aserciones (hoy se comparan a mano los registros) | Pendiente | _(completar)_ |
| **Pendiente:** historial con función de deshacer; prioridades en la cola | Pendiente | _(completar)_ |
| **Pendiente:** ciudades con nombres que incluyan `\|` (hoy se rechazan por el formato del archivo) | Pendiente | _(completar)_ |

## Declaración de uso de IA

Se consultó a Claude (Anthropic) para generar la primera versión del código, la estructura de módulos, los casos de prueba y este README, a partir de la guía de clase.
**Qué revisó y corrigió el equipo:** _(completar con honestidad: qué partes leyeron, ejecutaron, modificaron y pueden explicar)_

## Reglas de diseño aplicadas

- `main()` solo contiene el menú principal; la lógica vive en los módulos.
- Ninguna operación simula resultados con mensajes: cada opción modifica o consulta las estructuras.
- Una operación fallida nunca cierra el programa; las entradas se leen siempre como línea completa y se validan.
