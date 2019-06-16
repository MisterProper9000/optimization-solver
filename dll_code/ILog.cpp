#include "ILog.h"
#include <QFile>
#include <QTextStream>

QFile* file = 0;

int ILog::init(const char *fileName)
{
    if (!fileName)
        return ERR_WRONG_ARG;

    if (file) // file already inited
        return ERR_ANY_OTHER;

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return ERR_OPEN_ILogImpl;

    file->resize(0); // clear previous session log
    return ERR_OK;
}

int ILog::report(const char *msg)
{
    QTextStream stream(file);
    stream << msg << "\n" << endl;

    if (stream.status() != QTextStream::Ok)
        return ERR_WRITE_TO_ILogImpl;

    return ERR_OK;
}

void ILog::destroy()
{
    if(file)
    {
        file->close();
        delete file;
        file = 0;
    }
}


