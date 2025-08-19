/**
 * @file huffman.c
 * @author Paulo Sérgio Amorim Mônico (@paulosergioamorim)
 */

#include "huffman.h"
#include "utils.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

typedef Tree *(*table_fn)(Tree *);

static table_fn decodeTable[2] = {getLeftTree, getRightTree};

void helper_convertHuffmanTreeToTable(Tree *tree, unsigned int *table, unsigned int code)
{
    if (!tree)
        return;

    if (!isLeafTree(tree))
    {
        helper_convertHuffmanTreeToTable(getLeftTree(tree), table, code << 0x01);
        helper_convertHuffmanTreeToTable(getRightTree(tree), table, (code << 0x01) | 0x01);
        return;
    }

    unsigned char value = getValueTree(tree);
    table[value] = code;
}

unsigned int *convertHuffmanTreeToTable(Tree *tree)
{
    /*
     *  Salvar a codificação num inteiro com 31 bits de código é eficiente em uso de
     *  memória, mas causa erro se a árvore tiver uma altura maior que 31 (o que tem
     *  uma chance extremamente pequena de acontecer e requer uma distribuição de bytes
     *  sobremaneira desbalanceada).
     */
    if (getHeightTree(tree) > 31)
    {
        fprintf(stderr, "Árvore com altura maior que 31. Abortando.\n");
        exit(1);
    }

    unsigned int *table = calloc(ASCII_SIZE, sizeof(unsigned int));
    helper_convertHuffmanTreeToTable(tree, table, 0x01);

    return table;
}
void serializeHuffmanTree(Tree *tree, Bitmap *bitmap)
{
    if (!tree)
        return;

    if (isLeafTree(tree))
    {
        insertLSBBitmap(bitmap, 0x01);
        insertByteBitmap(bitmap, getValueTree(tree));
        return;
    }

    insertLSBBitmap(bitmap, 0x00);
    serializeHuffmanTree(getLeftTree(tree), bitmap);
    serializeHuffmanTree(getRightTree(tree), bitmap);
}

int getSerializedHuffmanTreeSize(Tree *tree)
{
    int leafNodesCount = getLeafNodesCountTree(tree);

    return leafNodesCount * 10 - 1; // 9 * leafs + 1 * (leafs - 1)
}

Tree *consumeBit(ReadBuffer *buffer, Bitmap *bitmap, Tree *huffmanTree, Tree *tree)
{
    unsigned char bit = bufferNextBit(buffer);

    if (!isLeafTree(huffmanTree))
        tree = decodeTable[bit](tree); // caso: raiz da árvore de huffman não é uma folha

    if (!isLeafTree(tree))
        return tree; // caso: avança para esquerda ou direita da árvore

    unsigned char value = getValueTree(tree);
    insertAlignedByteBitmap(bitmap, value);

    return huffmanTree; // caso: decodificou um byte e retorna para a raíz da árvore
}

Tree *createHuffmanTreeFromFile(ReadBuffer *buffer)
{
    int isLeafNode = bufferNextBit(buffer);

    if (isLeafNode)
    {
        unsigned char value = bufferNextByte(buffer);
        return createTree(value, 0);
    }

    Tree *tree = createTree(0, 0);

    setLeftTree(tree, createHuffmanTreeFromFile(buffer));
    setRightTree(tree, createHuffmanTreeFromFile(buffer));

    return tree;
}
