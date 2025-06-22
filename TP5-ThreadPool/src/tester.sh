#!/bin/bash

# Script para compilar y ejecutar tests personalizados del ThreadPool
# Incluye tanto los tests originales como los nuevos tests personalizados

echo "🧹 Limpieza inicial..."
make clean 2>/dev/null
rm -f test_custom_rama tpcustomtest 2>/dev/null

echo "🔧 Compilando tpcustomtest..."
if ! g++ -std=c++11 -Wall -g -pthread -o tpcustomtest tpcustomtest.cc thread-pool.cc Semaphore.cc; then
    echo "❌ Error en compilación"
    exit 1
fi

echo "✅ Compilación exitosa"

echo "🧪 Ejecutando tests personalizados..."

# Array de tests a ejecutar con descripción
declare -a tests=(
    "--s:Test simple original"
    "--single-thread-no-wait:Test single thread sin wait"
    "--single-thread-single-wait:Test single thread con wait"
    "--no-threads-double-wait:Test double wait sin threads"
    "--reuse-thread-pool:Test reutilización de pool"
    "--basic-scheduling:Test scheduling básico"
    "--sequential-timing:Test timing secuencial"
    "--atomic-stress:Test stress con atomics"
    "--multiple-waits:Test múltiples waits"
    "--concurrent-scheduling:Test scheduling concurrente"
    "--nested-scheduling:Test scheduling anidado"
    "--exception-handling:Test manejo de excepciones"
    "--performance-benchmark:Test benchmark de rendimiento"
)

# Contadores
total_tests=0
passed_tests=0

echo "=========================================="
echo "    EJECUTANDO TESTS PERSONALIZADOS"
echo "=========================================="

for test_entry in "${tests[@]}"; do
    flag="${test_entry%%:*}"
    description="${test_entry#*:}"
    
    echo
    echo "🔬 Ejecutando: $description"
    echo "   Comando: ./tpcustomtest $flag"
    echo "---"
    
    if timeout 30 ./tpcustomtest "$flag" 2>&1; then
        echo "✅ PASÓ: $description"
        ((passed_tests++))
    else
        exit_code=$?
        if [ $exit_code -eq 124 ]; then
            echo "⏰ TIMEOUT: $description (más de 30 segundos)"
        else
            echo "❌ FALLÓ: $description (código de salida: $exit_code)"
        fi
    fi
    
    ((total_tests++))
    echo "---"
done

echo
echo "=========================================="
echo "           RESUMEN DE TESTS"
echo "=========================================="
echo "Tests ejecutados: $total_tests"
echo "Tests que pasaron: $passed_tests"
echo "Tests que fallaron: $((total_tests - passed_tests))"

if [ $passed_tests -eq $total_tests ]; then
    echo "🎉 TODOS LOS TESTS PASARON! 🎉"
    echo "=========================================="
    
    echo
    echo "🏃 Ejecutando suite completa con --all-custom..."
    echo "---"
    if timeout 60 ./tpcustomtest --all-custom; then
        echo "✅ Suite completa ejecutada correctamente"
    else
        echo "❌ Error en suite completa"
    fi
else
    echo "❌ ALGUNOS TESTS FALLARON"
    echo "=========================================="
fi

echo
echo "🧹 Limpiando archivos temporales..."
rm -f tpcustomtest 2>/dev/null
echo "✅ Limpieza completada"
