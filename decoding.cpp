/***********************************************
Copyright (c) 2012 Piyush Kant
See the file license.txt for copying permission.
************************************************/

#include "decoding.h"
#include "ui_decoding.h"
#include "QFileDialog"
#include "QMessageBox"
#include "stdio.h"
#include "stdlib.h"
#include <assert.h>
#include "QDebug"
#include "QProgressDialog"
#include "choose.h"

Decoding::Decoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Decoding)
{
    ui->setupUi(this);

    /** Connect decompress dialog buttons **/
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseInputFile()));
    connect(ui->decodeButton, SIGNAL(clicked()), this, SLOT(decompressFile()));

    /** Warning initially hidden **/
    ui->warningText->setVisible(false);
}

Decoding::~Decoding()
{
    delete ui;
}

void Decoding::browseInputFile()
{
    /** Open select file dialog **/
    inputFileName = QFileDialog::getOpenFileName(this, tr("Open File"),"/home/piyush/Desktop",
                                                 tr("Binary File (*.bin)"));

    ui->inputFile->setText(inputFileName);
}

QString Decoding::getInputFileName()
{
    return inputFileName;
}

void Decoding::outputFilePath(const char *path, char * outputPath, const char *fileExtension)
{
    int i;
    const int pathLength = strlen(path);

    for(i=0; i<pathLength-4; i++)
    {
        outputPath[i] = path[i];
    }
    outputPath[i] = 0;

    strcat(outputPath, fileExtension);
}

void Decoding::decompressFile()
{
    if (inputFileName == "")
    {
        /** Warning **/
        ui->warningText->setVisible(false);
        showDoneMessage("Please select a file.");
    }
    else
    {
        ui->warningText->setVisible(false);

        QByteArray byteArray1 = inputFileName.toUtf8();
        const char* inputFile = byteArray1.constData();
        huffmanDecode(inputFile);
    }
}

void Decoding::showDoneMessage(const char * msg)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle("Decompression");
    msgBox.setIcon(QMessageBox::Information);
    QIcon icon(":/images/icon.png");
    msgBox.setWindowIcon(icon);
    msgBox.exec();
}

void Decoding::buildNodeList(HuffNode ** nodeList, HuffFreq * hFreq, unsigned int numOfFreq)
{
    unsigned int i;
    HuffNode * newNode;

    for (i = 0; i < numOfFreq; i++)
    {
        newNode = (HuffNode *) malloc(sizeof(HuffNode));
        newNode->charCode = hFreq[i].charCode;
        newNode->freq = hFreq[i].freq;
        newNode->next = NULL;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->leaf = true;

        addToNodeList(nodeList, newNode);
    }
}

void Decoding::addToNodeList(HuffNode ** nodeList, HuffNode * newNode)
{
    HuffNode * prevNode = NULL;
    HuffNode * currNode = *nodeList;

    while ((currNode != NULL && currNode->freq < newNode->freq))
    {
        prevNode = currNode;
        currNode = prevNode->next;
    }

    newNode->next = currNode;

    if (prevNode == NULL)
    {
        *nodeList = newNode;
    }
    else
    {
        prevNode->next = newNode;
    }
}

void Decoding::buildHuffTree(HuffNode ** nodeList)
{
    HuffNode * newNode, * leftNode, * rightNode;

    while((*nodeList)->next != NULL)
    {
        newNode = (HuffNode *)malloc(sizeof(HuffNode));

        leftNode = *nodeList;
        *nodeList = leftNode->next;

        rightNode = *nodeList;
        *nodeList = rightNode->next;

        newNode->charCode = 0;
        newNode->freq = leftNode->freq + rightNode->freq;
        newNode->next = NULL;
        newNode->left = leftNode;
        newNode->right = rightNode;
        newNode->leaf = false;

        addToNodeList(nodeList, newNode);
    }
}

void Decoding::writeDecodedData(FILE * src, FILE * dest, HuffNode * rootTree, unsigned int fileSize)
{
    int bit = -1;
    unsigned int c;
    unsigned int bytesWritten = 0;
    HuffNode * currNode = rootTree;
    bool cancel = false;
    unsigned int interval = fileSize/100;
    int progress = 1;

    QProgressDialog progressDialog("Decompressing...", "Cancel", 0, fileSize, this);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setValue(0);

    while(bytesWritten < fileSize)
    {
        if(bit < 0)
        {
            c = fgetc(src);

            if(c == EOF)
            {
                break;
            }

            bit = 7;
        }

        if((c >> bit) & 1)
        {
            currNode = currNode->right;
        }
        else
        {
            currNode = currNode->left;
        }

        if(currNode->leaf)
        {
            fputc(currNode->charCode, dest);
            bytesWritten++;
            currNode = rootTree;
        }

        bit--;

        if(bytesWritten > interval*progress)
        {
            progressDialog.setValue(interval*progress);
            progress++;
        }

        if (progressDialog.wasCanceled())
        {
            cancel = true;
            showDoneMessage("Aborted!");
            break;
        }
    }

    progressDialog.setValue(fileSize);

    if(!cancel)
    {
        showDoneMessage("File Decompressed Successfully!");
    }
}

void Decoding::freeHuffTree(HuffNode * treeRoot)
{
    if(treeRoot)
    {
        freeHuffTree(treeRoot->left);
        freeHuffTree(treeRoot->right);

        free(treeRoot);
    }
}

void Decoding::huffmanDecode(const char * inputFile)
{
    /** Open source file **/
    FILE *src = fopen(inputFile, "rb");

    /** Open destination file **/
    char outputPath[1000];
    const char * fileExtension = ".txt";
    outputFilePath(inputFile, outputPath, fileExtension);
    FILE * dest = fopen(outputPath, "wb");

    /** Check if file exist **/
    if (src == NULL)
    {
        fprintf(stderr, "Could not open file.");
        exit(EXIT_FAILURE);
    }

    /** Read Hufffman header **/
    HuffHeader hHeader;
    fread(&hHeader, sizeof(hHeader), 1, src);

    /** Read Huffman freq **/
    HuffFreq *hFreq = (HuffFreq *)calloc(hHeader.numOfFreq, sizeof(HuffFreq));
    fread(hFreq, sizeof(HuffFreq), hHeader.numOfFreq, src);

    /** Produce frequency node list **/
    HuffNode * nodeList = NULL;
    buildNodeList(&nodeList, hFreq, hHeader.numOfFreq);

    /** Build Huffman tree **/
    buildHuffTree(&nodeList);

    /** Write Huffman decoded data to destination file **/
    writeDecodedData(src, dest, nodeList, hHeader.fileSize);

    /** Free up memory **/
    freeHuffTree(nodeList);
    nodeList = NULL;
    free(hFreq);

    /** Close files **/
    fclose(src);
    fclose(dest);
}
