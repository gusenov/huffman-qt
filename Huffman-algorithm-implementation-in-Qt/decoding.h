#ifndef DECODING_H
#define DECODING_H

#include <QDialog>
#include "huffman.h"

namespace Ui {
class Decoding;
}

class Decoding : public QDialog
{
    Q_OBJECT
    
public:

    // Конструктор:
    explicit Decoding(QWidget *parent = 0);

    // Деструктор:
    ~Decoding();

    // Получить имя входного файла:
    QString getInputFileName();
    
private:

    // Пользовательский интерфейс:
    Ui::Decoding *ui;

    // Имя входного файла:
    QString inputFileName;

public slots:

    // Метод для выбора файла для восстановления:
    void browseInputFile();

    // Восстановить сжатый файл:
    void decompressFile();
};

#endif // DECODING_H
