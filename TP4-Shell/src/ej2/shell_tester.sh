#!/usr/bin/env bash
# -------------------------------------------------------------
#  Minimal test-suite for the "shell" binary of TP4.
#  Generates a concise, plagiarism‑free report.
# -------------------------------------------------------------

BIN="./shell"
RESULTS="results.txt"
TEMP_OUT=$(mktemp)
TEMP_ERR=$(mktemp)
EXPECTED=$(mktemp)
TESTFILE="test.txt"

# Fresh results file
: > "$RESULTS"

echo "Test run started: $(date)" | tee -a "$RESULTS"

# Helper to log a single line to console and file
log() {
  echo "$1" | tee -a "$RESULTS"
}

# Compile
if ! make -s; then
  log "[FATAL] Compilation failed. Aborting tests."
  exit 1
fi

if [ ! -x "$BIN" ]; then
  log "[FATAL] $BIN not found after compilation."
  exit 1
fi

# Auxiliary file for some tests
cat <<EOF > "$TESTFILE"
foo.png
bar.zip
baz.jpg
EOF

# Test bookkeeping
total=0; pass=0; fail=0; mem_ok=0; mem_leak=0

# ------------------------------------------------------------------
# run_test CMD DESC EXPECT_ERROR
# ------------------------------------------------------------------
run_test() {
  local cmd="$1"; local desc="$2"; local expect_err="$3"
  ((total++))
  local id=$(printf "%02d" "$total")
  
  # Execute our shell
  echo -e "$cmd\nexit" | $BIN > "$TEMP_OUT" 2> "$TEMP_ERR"
  local status=$?
  sed -i -E '/^Shell>.*/d' "$TEMP_OUT"

  local outcome="FAIL"
  if [[ "$expect_err" == "yes" ]]; then
    if [ "$status" -ne 0 ] || [ -s "$TEMP_ERR" ]; then
      outcome="PASS"; ((pass++))
    fi
  else
    # Reference output via bash
    bash -c "$cmd" > "$EXPECTED" 2>&1
    if diff -q "$EXPECTED" "$TEMP_OUT" >/dev/null; then
      outcome="PASS"; ((pass++))
    fi
  fi

  if [[ "$outcome" == "FAIL" ]]; then
    ((fail++))
  fi

  log "[${id}] ${desc} : ${outcome}"

  # Memory check (only for successful functional tests)
  if valgrind -q --leak-check=full --error-exitcode=42 echo -e "$cmd\nexit" | $BIN >/dev/null 2>&1; then
    ((mem_ok++))
  else
    ((mem_leak++))
  fi
}

# ---------------------
# Functional tests
# ---------------------
run_test "echo hola"                       "Echo simple"
run_test "echo 'hola mundo'"                "Echo con comillas"
run_test "seq 5 | head -n 1"               "Head de secuencia"
run_test "echo a   b   c | wc -w"          "Espacios múltiples + pipe"
run_test "cat $TESTFILE | grep \.zip$"     "Filtro por extensión"
run_test "yes | head -n 3"                 "Yes truncado"
run_test "exit"                            "Comando exit"

# Borde de argumentos
run_test "echo $(seq -s ' ' 1 63)"         "Límite exacto de 63 args"
run_test "echo $(seq -s ' ' 1 64)"         "Exceso de argumentos" yes

# Error de sintaxis
run_test "echo |"                           "Pipe sin rhs" yes
run_test "| echo hola"                     "Pipe sin lhs" yes

# Comando inválido
run_test "foobar_no_existe"                 "Comando inexistente" yes

# Pipeline largo (150 procesos)
PIPE="$(printf 'grep . | %.0s' {1..148})tail -n 1"
run_test "cat $TESTFILE | $PIPE"            "Pipeline de 150 grep + tail"

# ------------------------------------------------------------------
# Resumen
# ------------------------------------------------------------------
log "-------------------------------------------------------------"
log "TOTAL  : $total"
log "PASSED : $pass"
log "FAILED : $fail"
log "MEM OK : $mem_ok"
log "MEM LEAK: $mem_leak"
log "Fin de la corrida: $(date)"

# Cleanup
rm -f "$TEMP_OUT" "$TEMP_ERR" "$EXPECTED" "$TESTFILE"
make clean -s > /dev/null 2>&1
