/***********************************************
Copyright (c) 2012 Piyush Kant
See the file license.txt for copying permission.
************************************************/

#ifndef DECODING_H
#define DECODING_H

#include <QDialog>

namespace Ui {
class Decoding;
}

class Decoding : public QDialog
{
    Q_OBJECT
    
public:
    explicit Decoding(QWidget *parent = 0);
    ~Decoding();

    /** Input file name getter **/
    QString getInputFileName();

    /** Huffman encoding **/
    void huffmanDecode(const char * inputFile);
    
private:
    Ui::Decoding *ui;

    /** Input file name string **/
    QString inputFileName;

    /** Output file path **/
    void outputFilePath(const char * path, char * outputPath, const char * fileExtension);

    /** Get output file name **/
    void getOutputFileName();

    /** Show decompressing done message **/
    void showDoneMessage(const char * msg);

    /** Huffman node **/
    struct HuffNode
    {
        unsigned char charCode;
        unsigned int freq;
        bool leaf;
        HuffNode * next;
        HuffNode * left;
        HuffNode * right;
    };

    /** Huffman header node **/
    struct HuffHeader
    {
        unsigned int numOfFreq;
        unsigned int fileSize;
    };

    /** Huffman freq node **/
    struct HuffFreq
    {
        unsigned int freq;
        unsigned char charCode;
    };

    /** Build node list of decoded freq **/
    void buildNodeList(HuffNode ** nodeList, HuffFreq * hFreq, unsigned int numOfFreq);

    /** Add new Huffman node to node list **/
    void addToNodeList(HuffNode ** nodeList, HuffNode * newNode);

    /** Construct Huffman tree **/
    void buildHuffTree(HuffNode ** nodeList);

    /** Write Huffman decoded data to output file **/
    void writeDecodedData(FILE * src, FILE * dest, HuffNode * rootTree, unsigned int fileSize);

    /** Free Huffman tree memory **/
    void freeHuffTree(HuffNode * treeRoot);

public slots:

    /** Browse input file **/
    void browseInputFile();

    /** Compress file **/
    void decompressFile();
};

#endif // DECODING_H
