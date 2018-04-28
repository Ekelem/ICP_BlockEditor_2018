#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T11:10:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled13
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myarea.cpp

HEADERS  += mainwindow.h \
    myarea.h

FORMS    += mainwindow.ui

CONFIG += c++11

RESOURCES += \
    draggable.qrc
