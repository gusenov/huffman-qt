#-------------------------------------------------
#
# Project created by QtCreator 2012-07-02T00:13:53
#
#-------------------------------------------------

QT       += core gui

TARGET = HuffmanCoding
TEMPLATE = app


SOURCES += main.cpp\
        choose.cpp \
    encoding.cpp \
    decoding.cpp

HEADERS  += choose.h \
    encoding.h \
    decoding.h

FORMS    += choose.ui \
    encoding.ui \
    decoding.ui

RESOURCES += \
    Res.qrc

RC_FILE = AppIcon.rc
