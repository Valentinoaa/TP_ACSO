#!/bin/bash

# Compila el simulador
echo "Compilando src/sim..."
cd src && make && cd ..

# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo "Generando archivos .x..."
for file in inputs/*.s; do
    ./inputs/asm2hex "$file"
done

echo ""
echo "Probando todos los .x contra ref_sim_x86..."
echo "=========================================="

for file in inputs/*.x; do
    base=$(basename "$file" .x)
    echo "⏳ Test: $base"

    # Preparamos comandos para ambos simuladores
    echo "go" > tmp_cmds.txt
    echo "rdump" >> tmp_cmds.txt
    echo "quit" >> tmp_cmds.txt

    # Tu simulador
    ./src/sim "$file" < tmp_cmds.txt > sim_out.txt
    grep -A50 "Current register/bus values" sim_out.txt > sim_rdump.txt

    # Simulador de referencia
    ./ref_sim_x86 "$file" < tmp_cmds.txt > ref_out.txt
    grep -A50 "Current register/bus values" ref_out.txt > ref_rdump.txt

    if diff -q sim_rdump.txt ref_rdump.txt >/dev/null; then
        echo -e "${GREEN}✅ OK: $base${NC}"
    else
        echo -e "${RED}❌ Mismatch: $base${NC}"
        echo "⚠️  Diferencias:"
        diff -u sim_rdump.txt ref_rdump.txt
    fi

    echo "------------------------------------------"
done

# Limpieza
rm -f tmp_cmds.txt sim_out.txt ref_out.txt sim_rdump.txt ref_rdump.txt
