/***********************************************
Copyright (c) 2012 Piyush Kant
See the file license.txt for copying permission.
************************************************/

#include <QtGui/QApplication>
#include "choose.h"
#include "encoding.h"
#include "decoding.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Choose w;
    w.show();

    return a.exec();
}
