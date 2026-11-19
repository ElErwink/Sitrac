#!/bin/bash
# Ejecuta todos los casos y regenera la evidencia. Uso: desde la raíz del proyecto -> bash pruebas/ejecutar_pruebas.sh
make || exit 1
mkdir -p evidencia
for f in pruebas/*.in; do
  n=$(basename "$f" .in)
  echo "=================== $n ===================" > "evidencia/$n.txt"
  ./sitrac < "$f" >> "evidencia/$n.txt" 2>&1
  echo "Generado evidencia/$n.txt"
done
