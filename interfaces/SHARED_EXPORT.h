#ifndef __SHARED_EXPORT_H__
#define __SHARED_EXPORT_H__

#include <QtCore/qglobal.h>

#ifdef DLL_LIBRARY
    #define SHARED_EXPORT Q_DECL_EXPORT
#else
    #define SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif