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
    tlData.cpp \
    configdlg.cpp \
    reportdlg.cpp \
    tltools.cpp \
    dayviewdlg.cpp

HEADERS  += mainwindow.h \
    tlSettings.h \
    tlData.h \
    configdlg.h \
    reportdlg.h \
    tltools.h \
    dayviewdlg.h

FORMS    += mainwindow.ui \
    configdlg.ui \
    reportdlg.ui \
    dayviewdlg.ui

RESOURCES += \
    res.qrc
