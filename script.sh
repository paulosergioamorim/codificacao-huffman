#!/usr/bin/env bash

# Script para testar compactação e descompactação
# Suporta a flag --valgrind para análise de consumo de memória e registra logs e estatísticas em pasta específica

# Cores para saída
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Opções do Valgrind
LEAK_CHECK_OPTS="--leak-check=full --show-leak-kinds=definite --error-exitcode=1"
USE_VALGRIND=0

# Parse de flags
while [ "$1" != "" ]; do
  case "$1" in
    --valgrind)
      USE_VALGRIND=1
      ;;
    *)
      printf "%bERRO: Opção desconhecida: %s%b\n" "$RED" "$1" "$NC"
      exit 1
      ;;
  esac
  shift
done

# Configurações de caminhos
PROJECT_ROOT=$(pwd)
TESTS_DIR="$PROJECT_ROOT/tests"
LOGS_DIR="$TESTS_DIR/logs"
ENTRADA_DIR="$TESTS_DIR/entrada"
SAIDA_DIR="$PROJECT_ROOT/tests/saida"
COMPACTA_BIN="$PROJECT_ROOT/compacta"
DESCOMPACTA_BIN="$PROJECT_ROOT/descompacta"

# Verificar existência de diretórios e binários
for path in "$TESTS_DIR" "$ENTRADA_DIR" "$COMPACTA_BIN" "$DESCOMPACTA_BIN"; do
  if [ ! -e "$path" ]; then
    printf "%bERRO: Caminho não encontrado: %s%b\n" "$RED" "$path" "$NC"
    exit 1
  fi
done

# Criar pastas de logs e estatísticas
mkdir -p "$LOGS_DIR"
mkdir -p "$SAIDA_DIR"

rm -f "$LOGS_DIR"/*

# Processar cada arquivo de teste
for input_file in "$ENTRADA_DIR"/*; do
  [ -f "$input_file" ] || continue
  filename=$(basename "$input_file")
  printf "\nProcessando: %s\n" "$filename"

  # Compactação
  if [ $USE_VALGRIND -eq 1 ]; then
    log_file="$LOGS_DIR/${filename}_compacta_valgrind.log"
    valgrind $LEAK_CHECK_OPTS --log-file="$log_file" "$COMPACTA_BIN" "$input_file"
    valgrind_exit=$?
    if [ $valgrind_exit -eq 0 ]; then
      printf "%bValgrind: sem vazamentos na compactação de %s (log: %s)%b\n" "$GREEN" "$filename" "$log_file" "$NC"
    else
      printf "%bValgrind: vazamentos detectados na compactação de %s (exit=%d, log: %s)%b\n" "$RED" "$filename" $valgrind_exit "$log_file" "$NC"
    fi
    exit_code=$valgrind_exit
  else
    "$COMPACTA_BIN" "$input_file"
    exit_code=$?
  fi
  if [ $exit_code -ne 0 ]; then
    printf "%bERRO: Falha na compactação de %s (exit=%d)%b\n" "$RED" "$filename" $exit_code "$NC"
    continue
  fi

  # Mover arquivo compactado
  mv "$input_file.comp" "$SAIDA_DIR/"

  # Calcular estatísticas de compressão
  orig_size=$(wc -c < "$input_file")
  comp_size=$(wc -c < "$SAIDA_DIR/$filename.comp")
  if [ "$orig_size" -gt 0 ]; then
    rate=$(awk "BEGIN{printf \"%.2f\",(($orig_size-$comp_size)/$orig_size)*100}")
  else
    rate="N/A"
  fi

  # Gravar estatísticas em arquivo
  stats_file="$LOGS_DIR/${filename}_stats.log"
  printf "Tamanho original: %d bytes; compactado: %d bytes; taxa de compressao: %s%%\n" "$orig_size" "$comp_size" "$rate" > "$stats_file"

  # Descompactação
  cd "$SAIDA_DIR" || exit
  if [ $USE_VALGRIND -eq 1 ]; then
    log_file="$LOGS_DIR/${filename}_descompacta_valgrind.log"
    valgrind $LEAK_CHECK_OPTS --log-file="$log_file" "$DESCOMPACTA_BIN" "$filename.comp"
    valgrind_exit=$?
    if [ $valgrind_exit -eq 0 ]; then
      printf "%bValgrind: sem vazamentos na descompactacao de %s.comp (log: %s)%b\n" "$GREEN" "$filename" "$log_file" "$NC"
    else
      printf "%bValgrind: vazamentos detectados na descompactacao de %s.comp (exit=%d, log: %s)%b\n" "$RED" "$filename" $valgrind_exit "$log_file" "$NC"
    fi
    exit_code=$valgrind_exit
  else
    "$DESCOMPACTA_BIN" "$filename.comp"
    exit_code=$?
  fi
  if [ $exit_code -ne 0 ]; then
    printf "%bERRO: Falha na descompactacao de %s.comp (exit=%d)%b\n" "$RED" "$filename" $exit_code "$NC"
    cd - >/dev/null || exit
    continue
  fi

  # Verificação binária
  if diff "$input_file" "$SAIDA_DIR/$filename" > /dev/null; then
    printf "%bSUCESSO: Arquivos identicos - %s%b\n" "$GREEN" "$filename" "$NC"
  else
    printf "%bFALHA GRAVE: Diferencas encontradas em %s!%b\n" "$RED" "$filename" "$NC"
    diff "$input_file" "$SAIDA_DIR/$filename" | head -n 20
  fi

  # Limpeza e volta
  rm -f "$filename.comp"
  cd - >/dev/null || exit

done

printf "\n%bTestes concluídos!%b\n" "$GREEN" "$NC"
