#ifndef ILog_H
#define ILog_H

#include "error.h"
#include "SHARED_EXPORT.h"

class SHARED_EXPORT ILog
{
public:
    static int report(const char* msg);
    static int init(const char* fileName);
    static void destroy();
};


#endif // ILog_H
