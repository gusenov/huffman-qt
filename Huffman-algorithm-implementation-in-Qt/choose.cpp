#include "choose.h"
#include "ui_choose.h"
#include "encoding.h"
#include "decoding.h"

// Конструктор:
Choose::Choose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choose)
{
    ui->setupUi(this);

    // Связать клик на кнопку "Сжать данные" с методом showCompressDialog():
    connect(ui->compressButton, SIGNAL(clicked()), this, SLOT(showCompressDialog()));

    // Связать клик на кнопку "Восстановить данные" с методом showDecompressDialog():
    connect(ui->decompressButton, SIGNAL(clicked()), this, SLOT(showDecompressDialog()));
}

// Деструктор:
Choose::~Choose()
{
    delete ui;
}

// Показать диалоговое окно для сжатия данных:
void Choose::showCompressDialog()
{
    // Диалоговое окно для сжатия данных:
    Encoding* en = new Encoding(this);
    // Если в конструктор передавать указатель на родительский виджет,
    // то диалоговое окно будет отцентрировано относительно родителя.

    // Делаем размеры диалогового окна не изменяемыми:
    en->setFixedSize(en->size());

    en->setWindowTitle("Сжати данных по алгоритму Хаффмана");

    // Показать диалоговое окно:
    en->exec();
}

// Показать диалоговое окно для восстановления данных:
void Choose::showDecompressDialog()
{
    // Диалоговое окно для восстановления данных:
    Decoding* de = new Decoding(this);
    // Если в конструктор передавать указатель на родительский виджет,
    // то диалоговое окно будет отцентрировано относительно родителя.

    // Делаем размеры диалогового окна не изменяемыми:
    de->setFixedSize(de->size());

    de->setWindowTitle("Восстановление данных сжатых по алгоритму Хаффмана");

    // Показать диалоговое окно:
    de->exec();
}
