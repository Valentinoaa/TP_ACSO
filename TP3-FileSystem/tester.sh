#!/usr/bin/env bash

set -e

# -------------------------------------------------------------------
# TP3 Unix V6: Tester completo
# Compila, corre los tests, compara output, detecta mensajes extras
# y ejecuta Valgrind para validar memoria.
# -------------------------------------------------------------------

echo
echo "=== Limpiando y compilando ==="
make clean && make

echo
echo "=== Ejecutando tests completos del TP3 Unix V6 ==="
TESTDISK_DIR="./samples/testdisks"
IMAGES=("basicDiskImage" "depthFileDiskImage" "dirFnameSizeDiskImage")
ALL_PASSED=true

for IMG in "${IMAGES[@]}"; do
    echo
    echo "🧪 Test para imagen: $IMG"

    OUTPUT_FILE="output_${IMG}.txt"
    STDERR_FILE="stderr_${IMG}.log"
    GOLD_FILE="${TESTDISK_DIR}/${IMG}.gold"
    VALGRIND_FILE="valgrind_${IMG}.log"

    TEST_OK=true

    # ---------- EJECUCIÓN NORMAL ----------
    ./diskimageaccess -ip "${TESTDISK_DIR}/${IMG}" > "$OUTPUT_FILE" 2> "$STDERR_FILE"

    if diff -q "$OUTPUT_FILE" "$GOLD_FILE" >/dev/null; then
        echo "  ✔️  Output coincide con .gold"
    else
        echo "  ❌ Diferencias con .gold en $IMG"
        diff "$OUTPUT_FILE" "$GOLD_FILE" | head -n 5
        TEST_OK=false
    fi

    if [[ -s "$STDERR_FILE" ]]; then
        echo "  ⚠️  ADVERTENCIA: salida inesperada por stderr"
        head -n 3 "$STDERR_FILE"
        TEST_OK=false
    fi

    # ---------- EJECUCIÓN CON VALGRIND ----------
    valgrind --leak-check=full --error-exitcode=1 \
        ./diskimageaccess -ip "${TESTDISK_DIR}/${IMG}" \
        > /dev/null 2> "$VALGRIND_FILE"

    NO_LEAKS=$(grep -F "All heap blocks were freed -- no leaks are possible" "$VALGRIND_FILE")
    NO_ERRORS=$(grep -F "ERROR SUMMARY: 0 errors from 0 contexts" "$VALGRIND_FILE")

    if [[ -n "$NO_LEAKS" && -n "$NO_ERRORS" ]]; then
        echo "  ✔️  Valgrind: sin fugas ni errores"
    else
        echo "  ❌ Valgrind detectó problemas en $IMG"
        if [[ -z "$NO_LEAKS" ]]; then
            echo "     ⚠️  Fugas de memoria detectadas"
        fi
        if [[ -z "$NO_ERRORS" ]]; then
            echo "     ⚠️  Errores de acceso a memoria detectados"
        fi
        echo "     👉 Fragmento del log de Valgrind:"
        grep -A 5 -E "ERROR SUMMARY|leak-check summary|definitely lost" "$VALGRIND_FILE" | head -n 10
        TEST_OK=false
    fi

    if ! $TEST_OK; then
        ALL_PASSED=false
    fi
done

echo
if $ALL_PASSED; then
    echo "=== ✅ TODOS LOS TESTS PASARON CORRECTAMENTE ==="
else
    echo "=== ❌ ALGUNOS TESTS FALLARON O MOSTRARON PROBLEMAS DE MEMORIA ==="
    echo "    Revisá stderr_*.log y valgrind_*.log para más detalles."
fi

echo
echo "Limpiando archivos temporales..."
rm -f output_*.txt stderr_*.log valgrind_*.log
make clean