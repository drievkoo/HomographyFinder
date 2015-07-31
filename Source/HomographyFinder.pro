#-------------------------------------------------
#
# Project created by QtCreator 2015-07-31T19:34:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HomographyFinder
TEMPLATE = app

INCLUDEPATH += C:\Developer\opencv3\opencv\build\\include

LIBS += -LC:\Developer\opencv3\opencv\build\\x86\\vc12\\lib   \
-lopencv_ts300d       \
-lopencv_world300d

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h\
         homography.h

FORMS    += mainwindow.ui

DISTFILES += \
    INSTALL.txt \
    TODO.txt
