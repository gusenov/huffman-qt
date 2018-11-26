#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "stdio.h"

// Константы:
#define MAX_CODE_SIZE 32
#define CHARS_LIMIT 256

// Узел Хаффмана:
struct HuffNode
{
    unsigned int freq;  // частотность.
    unsigned char charCode;  // код символа.
    bool leaf;  // является ли узел листом?

    // Навигация по дереву:
    HuffNode* next;
    HuffNode* left;
    HuffNode* right;
};

// Таблица Хаффмана:
struct HuffCode
{
    unsigned char code[MAX_CODE_SIZE];
    int length;
};

// Заголовок Хаффмана:
struct HuffHeader
{
    unsigned int numOfFreq;  // количество частотностей.
    unsigned int fileSize;
};

// Частотность Хаффмана:
struct HuffFreq
{
    unsigned int freq;  // частотность.
    unsigned char charCode;  // код символа.
};

// Посчитать частотность всех символов и сохранить в списке:
void calcCharFreq(FILE* src, unsigned int* freqList);

// Построить список узлов Хаффмана (частотности сортируются по возрастанию):
void buildNodeList(HuffNode** nodeList, unsigned int* freqList);

// Построить список узлов из раскодированных частотностей:
void buildNodeList(HuffNode** nodeList, HuffFreq* hFreq, unsigned int numOfFreq);

// Добавить новый узел в список узлов Хаффмана (по возрастающему порядку):
void addToNodeList(HuffNode** nodeList, HuffNode* newNode);

// Построить дерево Хаффмана из списка узлов Хаффмана:
void buildHuffTree(HuffNode** nodeList);

// Построить таблицу перекодировки Хаффмана:
bool buildHuffCode(HuffNode* treeRoot, HuffCode* hCode, unsigned char goalChar);

// Вывести заголовок Хаффмана в выходной файл:
void writeHeader(FILE* dest, unsigned int numOfFreq, unsigned int fileSize);

// Вывести символ и его частотность в выходной файл:
void writeFreq(FILE* dest, unsigned int* freqList);

// Вывести закодированные по Хаффману данные в выходной файл:
void writeEncodedData(FILE* src, FILE* dest, HuffCode* huffCodeTable, unsigned int fileSize);

// Вывести раскодированные по Хаффману данные в выходной файл:
void writeDecodedData(FILE* src, FILE* dest, HuffNode* rootTree, unsigned int fileSize);

// Освободить память занятую по дерево Хаффмана:
void freeHuffTree(HuffNode* treeRoot);

// Закодировать файл по Хаффману:
void huffmanEncode(const char* inputFile);

// Раскодировать файл по Хаффману:
void huffmanDecode(const char * inputFile);

// Получить размер файла в байтах:
unsigned int getFileSize(FILE* src);

// Вычислить количество частотностей:
unsigned int calcNumOfFreq(unsigned int* freqList);

// Сформировать имя выходного файла:
void outputFilePath(const char* path, char* outputPath, const char *fileExtension);

#endif // HUFFMAN_H
