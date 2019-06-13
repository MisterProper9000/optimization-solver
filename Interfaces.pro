#-------------------------------------------------
#
# Project created by QtCreator 2019-04-28T14:04:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Interfaces
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Interfaces/util.cpp \
    Interfaces/IVectorImpl.cpp \
    Interfaces/ICompactImpl.cpp

HEADERS += \
    Interfaces/IBrocker.h \
    Interfaces/ICompact.h \
    Interfaces/IProblem.h \
    Interfaces/ISolver.h \
    Interfaces/IVector.h \
    Interfaces/ILog.h \
    Interfaces/error.h \
    Interfaces/ISet.h \
    Interfaces/SHARED_EXPORT.h
