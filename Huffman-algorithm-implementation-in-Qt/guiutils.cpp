#include "guiutils.h"
#include "QMessageBox"

// Показать информационное сообщение:
void showDoneMessage(const char* msg, const char* title)
{
    QMessageBox msgBox;
    msgBox.setText(msg);
    msgBox.setWindowTitle(title);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}
