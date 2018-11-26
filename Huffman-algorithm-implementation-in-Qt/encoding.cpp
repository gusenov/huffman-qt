#include "encoding.h"
#include "ui_encoding.h"
#include "QFileDialog"
#include "guiutils.h"

// Конструктор:
Encoding::Encoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Encoding)
{
    ui->setupUi(this);

    // Связать кнопку "Обзор…" с методом browseInputFile():
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseInputFile()));

    // Связать кнопку "Сжать данные" с методом compressFile():
    connect(ui->encodeButton, SIGNAL(clicked()), this, SLOT(compressFile()));
}

// Деструктор:
Encoding::~Encoding()
{
    delete ui;
}

// Метод для выбора файла для сжатия:
void Encoding::browseInputFile()
{
    // Открыть диалоговое окно для выбора файла:
    inputFileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"),
        QString(),
        tr("Текстовые файлы (*.txt)"));

    ui->inputFile->setText(inputFileName);
}

// Метод для сжатия файла:
void Encoding::compressFile()
{
    // Если имя файла пустое:
    if (inputFileName == "")
    {
        showDoneMessage("Пожалуйста, выберите файл.", "Сжатие данных");
    }

    // Иначе, если имя файла есть:
    else
    {
        // Имя файла:
        QByteArray byteArray = inputFileName.toUtf8();
        const char* inputFile = byteArray.constData();

        // Сжать файл:
        huffmanEncode(inputFile);
    }
}
