#!/usr/bin/env bash

set -e

# -------------------------------------------------------------------
# TP3 Unix V6: Script de pruebas
# Limpia, compila y ejecuta los tests sobre las imágenes de disco
# -------------------------------------------------------------------

echo
echo "=== Limpiando y compilando ==="
make clean && make

echo
echo "=== Ejecutando tests del TP3 Unix V6 ==="
TESTDISK_DIR="./samples/testdisks"
IMAGES=("basicDiskImage" "depthFileDiskImage" "dirFnameSizeDiskImage")
ALL_PASSED=true

for IMG in "${IMAGES[@]}"; do
    echo
    echo "Probando imagen: $IMG"
    OUTPUT="output_${IMG}.txt"
    GOLD="${TESTDISK_DIR}/${IMG}.gold"
    
    ./diskimageaccess -ip "${TESTDISK_DIR}/${IMG}" > "$OUTPUT" 2>/dev/null

    if diff -q "$OUTPUT" "$GOLD" >/dev/null; then
        echo "  Test PASÓ"
    else
        echo "  Test FALLÓ para $IMG"
        echo "  Diferencias (primeras 5 líneas):"
        diff "$OUTPUT" "$GOLD" | head -n 5
        ALL_PASSED=false
    fi
done

echo
if $ALL_PASSED; then
    echo "=== TODOS LOS TESTS PASARON ==="
else
    echo "=== ALGUNOS TESTS FALLARON ==="
fi

echo
echo "Limpiando archivos temporales..."
rm -f output_*.txt
make clean
