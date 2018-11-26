QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HuffmanCoding
TEMPLATE = app


SOURCES += main.cpp\
        choose.cpp \
    encoding.cpp \
    decoding.cpp \
    huffman.cpp \
    guiutils.cpp

HEADERS  += choose.h \
    encoding.h \
    decoding.h \
    huffman.h \
    guiutils.h

FORMS    += choose.ui \
    encoding.ui \
    decoding.ui
