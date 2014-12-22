#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T21:50:25
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTester
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           xmlreader.cpp \
           xmlwriter.cpp

HEADERS  += mainwindow.h \
            xmlreader.h \
            xmlwriter.h \
    globalenum.h

FORMS    += mainwindow.ui
