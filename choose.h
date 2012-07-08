/***********************************************
Copyright (c) 2012 Piyush Kant
See the file license.txt for copying permission.
************************************************/

#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>

namespace Ui {
class Choose;
}

class Choose : public QDialog
{
    Q_OBJECT
    
public:
    explicit Choose(QWidget *parent = 0);
    ~Choose();
    
private:
    Ui::Choose *ui;

private slots:
    void showCompressDialog();
    void showDecompressDialog();
};

#endif // CHOOSE_H
