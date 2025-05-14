#!/usr/bin/env bash

set -e

# -------------------------------------------------------------------
# TP3 Unix V6: Script de pruebas con Valgrind
# Ejecuta los tests sobre las imágenes de disco con Valgrind
# y verifica que no haya fugas de memoria
# -------------------------------------------------------------------

echo
echo "=== Ejecutando tests con Valgrind del TP3 Unix V6 ==="
TESTDISK_DIR="./samples/testdisks"
IMAGES=("basicDiskImage" "depthFileDiskImage" "dirFnameSizeDiskImage")
ALL_PASSED=true

for IMG in "${IMAGES[@]}"; do
    echo
    echo "Probando imagen con Valgrind: $IMG"
    
    # Prueba con opción -ip (en lugar de -qi)
    echo -n "  Test -ip: "
    VALGRIND_OUTPUT=$(valgrind --leak-check=full ./diskimageaccess -ip "${TESTDISK_DIR}/${IMG}" 2>&1)
    
    # Verificar si hay fugas de memoria
    if echo "$VALGRIND_OUTPUT" | grep -q "All heap blocks were freed -- no leaks are possible" && 
       ! echo "$VALGRIND_OUTPUT" | grep -q "ERROR SUMMARY: [^0]"; then
        echo "PASÓ (sin fugas de memoria)"
    else
        echo "FALLÓ (posibles fugas de memoria)"
        ALL_PASSED=false
    fi
    
    # Opcional: Prueba con opción adicional si es necesario
    # echo -n "  Test -qp: "
    # VALGRIND_OUTPUT=$(valgrind --leak-check=full ./diskimageaccess -qp "${TESTDISK_DIR}/${IMG}" 2>&1)
    # 
    # if echo "$VALGRIND_OUTPUT" | grep -q "All heap blocks were freed -- no leaks are possible" && 
    #    ! echo "$VALGRIND_OUTPUT" | grep -q "ERROR SUMMARY: [^0]"; then
    #     echo "PASÓ (sin fugas de memoria)"
    # else
    #     echo "FALLÓ (posibles fugas de memoria)"
    #     ALL_PASSED=false
    # fi
done

echo
if $ALL_PASSED; then
    echo "=== TODOS LOS TESTS DE VALGRIND PASARON ==="
else
    echo "=== ALGUNOS TESTS DE VALGRIND FALLARON ==="
fi