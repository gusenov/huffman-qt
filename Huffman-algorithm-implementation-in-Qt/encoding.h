#ifndef ENCODING_H
#define ENCODING_H

#include <QDialog>
#include "huffman.h"

namespace Ui {
class Encoding;
}

class Encoding : public QDialog
{
    Q_OBJECT
    
public:

    // Конструктор:
    explicit Encoding(QWidget *parent = 0);

    // Деструктор:
    ~Encoding();

    // Получить имя входного файла:
    QString getInputFileName();

private:

    // Пользовательский интерфейс:
    Ui::Encoding *ui;

    // Имя входного файла:
    QString inputFileName;

public slots:

    // Метод для выбора файла для сжатия:
    void browseInputFile();

    // Метод для сжатия файла:
    void compressFile();
};

#endif // ENCODING_H
