#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T21:17:09
#
#-------------------------------------------------
TARGET = TimeLog
TEMPLATE = app
VERSION = "0.0.8.0_beta"

RC_ICONS = "resource/TimeLog.ico"

QMAKE_TARGET_COMPANY = "Marco Hartung"
QMAKE_TARGET_PRODUCT = "TimeLog"
QMAKE_TARGET_DESCRIPTION = "TimeLog"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2019 Marco Hartung"

DEFINES += \
  APP_VERSION=\"\\\"$$VERSION\\\"\" \
  APP_COPYRIGHT=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\" \
  APP_NAME=\\\"$$TARGET\\\"

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp\
        mainwindow.cpp \
    tlSettings.cpp \
    tlData.cpp \
    configdlg.cpp \
    reportdlg.cpp \
    tltools.cpp \
    dayviewdlg.cpp \
    aboutdlg.cpp

HEADERS  += mainwindow.h \
    tlSettings.h \
    tlData.h \
    configdlg.h \
    reportdlg.h \
    tltools.h \
    dayviewdlg.h \
    aboutdlg.h

FORMS    += mainwindow.ui \
    configdlg.ui \
    reportdlg.ui \
    dayviewdlg.ui \
    aboutdlg.ui

RESOURCES += \
    res.qrc
