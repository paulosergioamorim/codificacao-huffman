#!/usr/bin/env bash

# Script para testar compactação e descompactação
# Suporta a flag --valgrind para análise de consumo de memória

# Cores para saída
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Parse de flags
VALGRIND_CMD=""
while [ "$1" != "" ]; do
  case "$1" in
    --valgrind)
      # Forçar logs do Valgrind para STDOUT
      VALGRIND_CMD="valgrind"
      ;;
    *)
      printf "${RED}ERRO: Opção desconhecida: %s${NC}\n" "$1"
      exit 1
      ;;
  esac
  shift
done

# Configurações
PROJECT_ROOT=$(pwd)
TESTS_DIR="$PROJECT_ROOT/tests"
ENTRADA_DIR="$TESTS_DIR/entrada"
SAIDA_DIR="$TESTS_DIR/saida"
COMPACTA_BIN="$PROJECT_ROOT/compacta"
DESCOMPACTA_BIN="$PROJECT_ROOT/descompacta"

make clean
make all

# Verificar existência dos diretórios e binários
if [ ! -d "$TESTS_DIR" ]; then
    printf "${RED}ERRO: Diretório 'tests' não encontrado!${NC}\n"
    exit 1
fi

if [ ! -d "$ENTRADA_DIR" ]; then
    printf "${RED}ERRO: Diretório de entrada '%s' não existe!${NC}\n" "$ENTRADA_DIR"
    exit 1
fi

mkdir -p "$SAIDA_DIR"

if [ ! -f "$COMPACTA_BIN" ]; then
    printf "${RED}ERRO: Binário 'compacta' não encontrado em %s!${NC}\n" "$PROJECT_ROOT"
    exit 1
fi

if [ ! -f "$DESCOMPACTA_BIN" ]; then
    printf "${RED}ERRO: Binário 'descompacta' não encontrado em %s!${NC}\n" "$PROJECT_ROOT"
    exit 1
fi

# Processar cada arquivo de teste
for input_file in "$ENTRADA_DIR"/*; do
    [ -f "$input_file" ] || continue
    filename=$(basename "$input_file")
    printf "\nProcessando: %s\n" "$filename"

    # Etapa 1: Compactar
    eval "$VALGRIND_CMD '$COMPACTA_BIN' '$input_file'"
    compact_exit=$?
    if [ -n "$VALGRIND_CMD" ]; then
        if [ $compact_exit -eq 0 ]; then
            printf "${GREEN}Valgrind: sem erros na compactação de %s${NC}\n" "$filename"
        else
            printf "${RED}Valgrind: erros detectados na compactação de %s${NC}\n" "$filename"
        fi
    fi
    if [ $compact_exit -ne 0 ]; then
        printf "${RED}ERRO: Falha na compactação de %s${NC}\n" "$filename"
        continue
    fi

    compressed_file="$input_file.comp"
    if [ ! -f "$compressed_file" ]; then
        printf "${RED}ERRO: Arquivo compactado não gerado: %s.comp${NC}\n" "$filename"
        continue
    fi

    # Etapa 2: Mover para pasta de saída
    mv "$compressed_file" "$SAIDA_DIR/"

    # Mostrar tamanhos e taxa
    orig_size=$(wc -c < "$input_file")
    comp_size=$(wc -c < "$SAIDA_DIR/$filename.comp")
    printf "Tamanho original: %d bytes; compactado: %d bytes\n" "$orig_size" "$comp_size"
    if [ "$orig_size" -gt 0 ]; then
        compression_rate=$(awk "BEGIN {printf \"%.2f\", (($orig_size - $comp_size) / $orig_size) * 100}")
        printf "Taxa de compressão: %s%%\n" "$compression_rate"
    else
        printf "Taxa de compressão: N/A\n"
    fi

    # Etapa 3: Descompactar
    cd "$SAIDA_DIR" || exit
    eval "$VALGRIND_CMD '$DESCOMPACTA_BIN' '$filename.comp'"
    decompress_exit=$?
    if [ -n "$VALGRIND_CMD" ]; then
        if [ $decompress_exit -eq 0 ]; then
            printf "${GREEN}Valgrind: sem erros na descompactação de %s.comp${NC}\n" "$filename"
        else
            printf "${RED}Valgrind: erros detectados na descompactação de %s.comp${NC}\n" "$filename"
        fi
    fi
    if [ $decompress_exit -ne 0 ]; then
        printf "${RED}ERRO: Falha na descompactação de %s.comp${NC}\n" "$filename"
        cd - >/dev/null || exit
        continue
    fi

    # Etapa 4: Verificação
    descompressed_file="$SAIDA_DIR/$filename"
    if [ ! -f "$descompressed_file" ]; then
        printf "${RED}ERRO: Arquivo descompactado não gerado: %s${NC}\n" "$filename"
        cd - >/dev/null || exit
        continue
    fi

    # Comparação binária
    if diff "$input_file" "$descompressed_file" > /dev/null; then
        printf "${GREEN}SUCESSO: Arquivos idênticos - %s${NC}\n" "$filename"
    else
        printf "${RED}FALHA GRAVE: Diferenças encontradas em %s!${NC}\n" "$filename"
        diff "$input_file" "$descompressed_file" | head -n 20
    fi

    # Limpeza
    rm -f "$SAIDA_DIR/$filename.comp"
    cd - >/dev/null || exit

done

printf "${GREEN}Testes concluídos!${NC}\n"
