#include "decoding.h"
#include "ui_decoding.h"
#include "QFileDialog"
#include "guiutils.h"

// Конструктор:
Decoding::Decoding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Decoding)
{
    ui->setupUi(this);

    // Связать кнопку "Обзор…" с методом browseInputFile():
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browseInputFile()));

    // Связать кнопку "Восстановить данные" с методом decompressFile():
    connect(ui->decodeButton, SIGNAL(clicked()), this, SLOT(decompressFile()));
}

// Деструктор:
Decoding::~Decoding()
{
    delete ui;
}

// Метод для выбора файла для восстановления:
void Decoding::browseInputFile()
{
    // Открыть диалоговое окно для выбора файла:
    inputFileName = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"),
        QString(),
        tr("Бинарные файлы (*.bin)"));

    ui->inputFile->setText(inputFileName);
}

// Восстановить сжатый файл:
void Decoding::decompressFile()
{
    // Если имя файла пустое:
    if (inputFileName == "")
    {
        showDoneMessage("Пожалуйста, выберите файл.", "Восстановление данных");
    }

    // Иначе, если имя файла есть:
    else
    {
        // Имя файла:
        QByteArray byteArray = inputFileName.toUtf8();
        const char* inputFile = byteArray.constData();

        // Сжать файл:
        huffmanDecode(inputFile);
    }
}
