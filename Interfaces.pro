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
    dll_code/IVectorImpl.cpp \
    dll_code/ISetImpl.cpp

HEADERS += \
    IVector.h \
    ILog.h \
    error.h \
    ISet.h \
    SHARED_EXPORT.h

