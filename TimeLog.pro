#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T21:17:09
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeLog
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tlSettings.cpp \
    tlData.cpp

HEADERS  += mainwindow.h \
    tlSettings.h \
    tlData.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
