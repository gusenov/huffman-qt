/***********************************************
Copyright (c) 2012 Piyush Kant
See the file license.txt for copying permission.
************************************************/

#include "choose.h"
#include "ui_choose.h"
#include "encoding.h"
#include "decoding.h"

Choose::Choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choose)
{
    ui->setupUi(this);

    /** Connect Compress File button to open compress dialog **/
    connect(ui->compressButton, SIGNAL(clicked()), this, SLOT(showCompressDialog()));

    /** Connect Decompress File button to open decompress dialog **/
    connect(ui->decompressButton, SIGNAL(clicked()), this, SLOT(showDecompressDialog()));
}

Choose::~Choose()
{
    delete ui;
}

void Choose::showCompressDialog()
{
    QDialog parent;
    Encoding * en;
    en = new Encoding(&parent);
    en->setFixedSize(en->size());
    en->setWindowTitle("Huffman Encoding");
    en->exec();
}

void Choose::showDecompressDialog()
{
    QDialog parent;
    Decoding * de;
    de = new Decoding(&parent);
    de->setFixedSize(de->size());
    de->setWindowTitle("Huffman Decoding");
    de->exec();
}
