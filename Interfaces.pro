#-------------------------------------------------
#
# Project created by QtCreator 2019-04-28T14:04:47
#
#-------------------------------------------------

QT       += gui sql printsupport

TARGET   =  Interfaces
CONFIG   += console

TEMPLATE = app


SOURCES += main.cpp \
    BrokerImpl.cpp\
    ISet.cpp \
    controller.cpp \
    sqlconnectiondialog.cpp

HEADERS += \
    Interfaces/IBrocker.h \
    Interfaces/ICompact.h \
    Interfaces/IProblem.h \
    Interfaces/ISolver.h \
    Interfaces/IVector.h \
    Interfaces/ILog.h \
    Interfaces/error.h \
    Interfaces/ISet.h \
    Interfaces/SHARED_EXPORT.h \
    controller.h \
    sqlconnectiondialog.h

FORMS += \
    controller.ui \
    sqlconnectiondialog.ui


































