#include <QApplication>
#include "choose.h"
#include "encoding.h"
#include "decoding.h"

// Точка входа в программу:
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Создание и отображение диалогового окна с двумя кнопками
    // "Сжать данные" и "Восстановить данные":
    Choose w;
    w.show();

    return a.exec();
}
