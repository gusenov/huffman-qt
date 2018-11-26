#include "huffman.h"
#include "QProgressDialog"
#include "guiutils.h"

// Посчитать частотность всех символов и сохранить в списке:
void calcCharFreq(FILE* src, unsigned int* freqList)
{
    int c;  // символ.

    // Считываем очередной символ из файла:
    while((c = fgetc(src)) != EOF)
    {
        freqList[c]++;
    }

    // Перемещаемся в начало файла:
    rewind(src);
}

// Построить список узлов Хаффмана (частотности сортируются по возрастанию):
void buildNodeList(HuffNode** nodeList, unsigned int* freqList)
{
    unsigned int i;
    HuffNode* newNode;

    for (i = 0; i < CHARS_LIMIT; i++)
    {
        // Если частотность символа больше 0,
        // т.е. символ встречался во входном файле:
        if (freqList[i] > 0)
        {
            // Создание нового узла дерева Хаффмана:
            newNode = (HuffNode *)calloc(1, sizeof(HuffNode));
            newNode->charCode = i;
            newNode->freq = freqList[i];
            newNode->next = NULL;
            newNode->left = NULL;
            newNode->right = NULL;
            newNode->leaf = true;

            // Добавление нового узла в дерево Хаффмана:
            addToNodeList(nodeList, newNode);
        }
    }
}

// Построить список узлов из раскодированных частотностей:
void buildNodeList(HuffNode** nodeList, HuffFreq* hFreq, unsigned int numOfFreq)
{
    unsigned int i;
    HuffNode* newNode;

    for (i = 0; i < numOfFreq; i++)
    {
        newNode = (HuffNode *)malloc(sizeof(HuffNode));
        newNode->charCode = hFreq[i].charCode;
        newNode->freq = hFreq[i].freq;
        newNode->next = 0;
        newNode->left = 0;
        newNode->right = 0;
        newNode->leaf = true;

        addToNodeList(nodeList, newNode);
    }
}


// Добавить новый узел в список узлов Хаффмана (по возрастающему порядку):
void addToNodeList(HuffNode** nodeList, HuffNode* newNode)
{
    HuffNode* prevNode = NULL;  // предыдущий узел.
    HuffNode* currNode = *nodeList;  // текущий узел.

    while(currNode != NULL && currNode->freq < newNode->freq)
    {
        prevNode = currNode;
        currNode = prevNode->next;
    }

    newNode->next = currNode;

    if(prevNode == NULL)
    {
        *nodeList = newNode;
    }
    else
    {
        prevNode->next = newNode;
    }
}

// Построить дерево Хаффмана из списка узлов Хаффмана:
void buildHuffTree(HuffNode** nodeList)
{
    HuffNode* leftNode, *rightNode;
    HuffNode* newNode;

    while ((*nodeList)->next)
    {
        leftNode = *nodeList;
        *nodeList = leftNode->next;

        rightNode = *nodeList;
        *nodeList = rightNode->next;

        newNode = (HuffNode *)calloc(1, sizeof(HuffNode));
        newNode->charCode = 0;
        newNode->freq = leftNode->freq + rightNode->freq;
        newNode->next = NULL;
        newNode->left = leftNode;
        newNode->right = rightNode;
        newNode->leaf = false;

        addToNodeList(nodeList, newNode);
    }
}

// Построить таблицу перекодировки Хаффмана:
bool buildHuffCode(HuffNode* treeRoot, HuffCode* hCode, unsigned char goalChar)
{
    if (treeRoot->charCode == goalChar && treeRoot->leaf)
    {
        return true;
    }

    if (treeRoot->left)
    {
        hCode->code[hCode->length] = '0';
        hCode->length++;

        if (hCode->length == MAX_CODE_SIZE)
        {
            printf("Code size exceeds limit!");
            return false;
        }

        if (buildHuffCode(treeRoot->left, hCode, goalChar))
        {
            hCode->code[hCode->length] = 0;
            return true;
        }
        else
        {
            hCode->length--;
            hCode->code[hCode->length] = 0;
        }
    }

    if (treeRoot->right)
    {
        hCode->code[hCode->length] = '1';
        hCode->length++;

        if (buildHuffCode(treeRoot->right, hCode, goalChar))
        {
            return true;
        }
        else
        {
            hCode->length--;
            hCode->code[hCode->length] = 0;
        }
    }

    return false;
}

// Вывести заголовок Хаффмана в выходной файл:
void writeHeader(FILE* dest, unsigned int numOfFreq, unsigned int fileSize)
{
    HuffHeader hHeader;

    hHeader.numOfFreq = numOfFreq;  // количество частотностей.
    hHeader.fileSize = fileSize;  // размер файла.

    fwrite(&hHeader, sizeof(hHeader), 1, dest);
}

// Вывести символ и его частотность в выходной файл:
void writeFreq(FILE* dest, unsigned int* freqList)
{
    unsigned int i;
    HuffFreq hFreq;
    for (i = 0; i < CHARS_LIMIT; i++)
    {
        // Если частотность символа больше 0,
        // т.е. символ встречался во входном файле:
        if (freqList[i] > 0)
        {
            hFreq.charCode = i;  // код символа.
            hFreq.freq = freqList[i];  // частотность символа.

            fwrite(&hFreq, sizeof(HuffFreq), 1, dest);
        }
    }
}

// Вывести закодированные по Хаффману данные в выходной файл:
void writeEncodedData(FILE* src, FILE* dest, HuffCode* huffCodeTable, unsigned int fileSize)
{
    int i, c;
    unsigned int bits = 0;
    char currChar = 0;
    HuffCode currCode;
    bool cancel = false;
    unsigned int interval = fileSize/100;
    int progress = 1;
    unsigned int bytes = 0;

    QProgressDialog progressDialog("Сжатие данных...", "Отмена", 0, fileSize, nullptr);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setValue(0);

    while ((c = fgetc(src)) != EOF)
    {
        bytes++;
        currCode = huffCodeTable[c];

        for (i = 0; i < currCode.length; i++)
        {
            currChar = (currChar << 1) + (currCode.code[i] == '1' ? 1 : 0);
            bits++;

            if(bits == 8)
            {
                fputc(currChar, dest);
                currChar = 0;
                bits = 0;
            }
        }


        if (bytes > interval * progress)
        {
            progressDialog.setValue(progress);
            progress++;
        }

        if (progressDialog.wasCanceled())
        {
            cancel = true;
            showDoneMessage("Отменено!", "Сжатие данных");
            break;
        }
    }

    if (bits > 0)
    {
        currChar = currChar << (8 - bits);
        fputc(currChar, dest);
    }

    progressDialog.setValue(fileSize);

    if (!cancel)
    {
        showDoneMessage("Файл успешно сжат!", "Сжатие данных");
    }
}

// Вывести раскодированные по Хаффману данные в выходной файл:
void writeDecodedData(FILE * src, FILE * dest, HuffNode * rootTree, unsigned int fileSize)
{
    int bit = -1;
    int c;
    unsigned int bytesWritten = 0;
    HuffNode * currNode = rootTree;
    bool cancel = false;
    unsigned int interval = fileSize/100;
    int progress = 1;

    QProgressDialog progressDialog("Восстановление данных...", "Отмена", 0, fileSize, nullptr);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setValue(0);

    while (bytesWritten < fileSize)
    {
        if (bit < 0)
        {
            c = fgetc(src);

            if (c == EOF)
            {
                break;
            }

            bit = 7;
        }

        if ((c >> bit) & 1)
        {
            currNode = currNode->right;
        }
        else
        {
            currNode = currNode->left;
        }

        if (currNode->leaf)
        {
            fputc(currNode->charCode, dest);
            bytesWritten++;
            currNode = rootTree;
        }

        bit--;

        if (bytesWritten > interval*progress)
        {
            progressDialog.setValue(interval*progress);
            progress++;
        }

        if (progressDialog.wasCanceled())
        {
            cancel = true;
            showDoneMessage("Омененно!", "Восстановление данных");
            break;
        }
    }

    progressDialog.setValue(fileSize);

    if(!cancel)
    {
        showDoneMessage("Файл успешно восстановлен!", "Восстановление данных");
    }
}

// Освободить память занятую по дерево Хаффмана:
void freeHuffTree(HuffNode* treeRoot)
{
    if (treeRoot)  // если узел не пустой.
    {
        freeHuffTree(treeRoot->left);  // удаляем левую часть.
        freeHuffTree(treeRoot->right);  // удаляем правую часть.

        // После того как левая и правая часть удалены,
        // удаляем сам узел:
        free(treeRoot);
    }
}

// Закодировать файл по Хаффману:
void huffmanEncode(const char* inputFile)
{
    // Открыть исходный файл:
    FILE* src = fopen(inputFile, "rb");

    // Проверка на существование входного файла:
    if (src == NULL)
    {
        fprintf(stderr, "Невозможно открыть входной файл.");
        exit(EXIT_FAILURE);
    }

    // Открыть выходной файл:
    char outputPath[1000];
    const char * fileExtension = ".bin";
    outputFilePath(inputFile, outputPath, fileExtension);
    FILE * dest = fopen(outputPath, "wb");

    // Получить размер входного файла:
    unsigned int fileSize;
    fileSize = getFileSize(src);

    // Посчитать частотности символов:
    unsigned int * freqList;
    freqList = (unsigned int *)calloc(CHARS_LIMIT, sizeof(unsigned int));
    calcCharFreq(src, freqList);

    // Посчитать количество частотностей
    // (т.е. тех которые больше нуля):
    unsigned int numOfFreq;
    numOfFreq = calcNumOfFreq(freqList);

    // Построить список Хаффмана (частотности отсортированные по возрастанию):
    HuffNode* nodeList = NULL;
    buildNodeList(&nodeList, freqList);

    // Построить дерево Хаффмана:
    buildHuffTree(&nodeList);
    HuffNode * treeRoot = nodeList;

    // Построить таблицу Хаффмана:
    unsigned int i;
    HuffCode newCode;
    HuffCode * huffCodeTable;
    huffCodeTable = (HuffCode *)calloc(CHARS_LIMIT, sizeof(HuffCode));
    for(i=0; i<CHARS_LIMIT; i++)
    {
        if(freqList[i] > 0)
        {
            newCode.length = 0;
            buildHuffCode(treeRoot, &newCode, i);
            huffCodeTable[i] = newCode;
        }
    }

    // Записать заголовок:
    writeHeader(dest, numOfFreq, fileSize);

    // Записать частотности:
    writeFreq(dest, freqList);

    // Вывести закодированные по Хаффману данные в выходной файл:
    writeEncodedData(src, dest, huffCodeTable, fileSize);

    // Освободить память:
    freeHuffTree(treeRoot);
    treeRoot = NULL;
    free(huffCodeTable);
    free(freqList);

    // Закрыть файлы:
    fclose(src);
    fclose(dest);
}

// Раскодировать файл по Хаффману:
void huffmanDecode(const char * inputFile)
{
    // Открыть исходный файл:
    FILE *src = fopen(inputFile, "rb");

    // Проверка на существование входного файла:
    if (src == NULL)
    {
        fprintf(stderr, "Невозможно открыть входной файл.");
        exit(EXIT_FAILURE);
    }

    // Открыть выходной файл:
    char outputPath[1000];
    const char* fileExtension = ".txt";
    outputFilePath(inputFile, outputPath, fileExtension);
    FILE* dest = fopen(outputPath, "wb");

    // Считать заголовок:
    HuffHeader hHeader;
    fread(&hHeader, sizeof(hHeader), 1, src);

    // Считать частотности:
    HuffFreq* hFreq = (HuffFreq *)calloc(hHeader.numOfFreq, sizeof(HuffFreq));
    fread(hFreq, sizeof(HuffFreq), hHeader.numOfFreq, src);

    // Построить список частотностей:
    HuffNode* nodeList = NULL;
    buildNodeList(&nodeList, hFreq, hHeader.numOfFreq);

    // Построить дерево Хаффмана:
    buildHuffTree(&nodeList);

    // Вывести раскодированные по Хаффману данные в выходной файл:
    writeDecodedData(src, dest, nodeList, hHeader.fileSize);

    // Освободить память:
    freeHuffTree(nodeList);
    nodeList = NULL;
    free(hFreq);

    // Закрыть файлы:
    fclose(src);
    fclose(dest);
}

// Получить размер файла в байтах:
unsigned int getFileSize(FILE* src)
{
    unsigned int fileSize = 0;
    int c;

    while ((c = fgetc(src)) != EOF)
    {
        fileSize++;
    }
    rewind(src);

    return fileSize;
}

// Вычислить количество частотностей:
unsigned int calcNumOfFreq(unsigned int* freqList)
{
    unsigned int i;
    unsigned int numOfFreq = 0;

    for (i = 0; i < CHARS_LIMIT; i++)
    {
        if (freqList[i] > 0)
        {
            numOfFreq++;
        }
    }

    return numOfFreq;
}

// Сформировать имя выходного файла:
void outputFilePath(const char *path, char * outputPath, const char *fileExtension)
{
    int i;
    const int pathLength = strlen(path);

    for (i = 0; i < pathLength - 4; i++)
    {
        outputPath[i] = path[i];
    }
    outputPath[i] = 0;

    strcat(outputPath, fileExtension);
}
