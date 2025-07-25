#!/bin/bash

# Script para testar compactação e descompactação

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
    echo "ERRO: Diretório 'tests' não encontrado!"
    exit 1
fi

if [ ! -d "$ENTRADA_DIR" ]; then
    echo "ERRO: Diretório de entrada '$ENTRADA_DIR' não existe!"
    exit 1
fi

mkdir -p "$SAIDA_DIR"  # Garante que o diretório de saída existe

if [ ! -f "$COMPACTA_BIN" ]; then
    echo "ERRO: Binário 'compacta' não encontrado em $PROJECT_ROOT!"
    exit 1
fi

if [ ! -f "$DESCOMPACTA_BIN" ]; then
    echo "ERRO: Binário 'descompacta' não encontrado em $PROJECT_ROOT!"
    exit 1
fi

# Processar cada arquivo de teste
for input_file in "$ENTRADA_DIR"/*; do
    if [ -f "$input_file" ]; then
        echo "Processando: $(basename "$input_file")"
        filename=$(basename "$input_file")
        
        # Etapa 1: Compactar
        "$COMPACTA_BIN" "$input_file"
        if [ $? -ne 0 ]; then
            echo "ERRO: Falha na compactação de $filename"
            continue
        fi
        
        compressed_file="$input_file.comp"
        if [ ! -f "$compressed_file" ]; then
            echo "ERRO: Arquivo compactado não gerado: $filename.comp"
            continue
        fi
        
        # Etapa 2: Mover para pasta de saída
        mv "$compressed_file" "$SAIDA_DIR/"
        
        # Etapa 3: Descompactar
        cd "$SAIDA_DIR" || exit
        "$DESCOMPACTA_BIN" "$filename.comp"
        if [ $? -ne 0 ]; then
            echo "ERRO: Falha na descompactação de $filename.comp"
            cd - >/dev/null || exit
            continue
        fi
        
        # Etapa 4: Verificação
        descompressed_file="$SAIDA_DIR/$filename"
        if [ ! -f "$descompressed_file" ]; then
            echo "ERRO: Arquivo descompactado não gerado: $filename"
            cd - >/dev/null || exit
            continue
        fi
        
        # Comparação binária
        diff_result=$(diff "$input_file" "$descompressed_file" 2>&1)
        if [ $? -eq 0 ]; then
            echo "SUCESSO: Arquivos idênticos - $filename"
        else
            echo "FALHA GRAVE: Diferenças encontradas em $filename!"
            echo "$diff_result" | head -n 20  # Mostra primeiras 20 linhas do diff
        fi
        
        # Limpeza e retorno
        rm -f "$SAIDA_DIR/$filename.comp"  # Remove arquivo compactado
        cd - >/dev/null || exit
    fi
done

echo "Testes concluídos!"