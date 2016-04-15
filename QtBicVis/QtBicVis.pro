#-------------------------------------------------
#
# Project created by QtCreator 2016-04-15T11:59:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtBicVis
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ccell.cpp \
    qbiclusterwindow.cpp \
    qcustomplot.cpp \
    common.cpp \
    qhistogram.cpp \
    qplotwindow.cpp \
    qsigmawindow.cpp \
    qbicstats.cpp \
    qbicwin.cpp \
    qparallelplot.cpp

HEADERS  += mainwindow.h \
    ccell.h \
    qbiclusterwindow.h \
    common.h \
    qcustomplot.h \
    qhistogram.h \
    qplotwindow.h \
    qsigmawindow.h \
    qbicstats.h \
    qbicwin.h \
    qparallelplot.h
QT += printsupport

INCLUDEPATH += "D:\Programowanie\boost_1_60_0"
