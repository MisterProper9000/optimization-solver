#ifndef ISOLVER_H
#define ISOLVER_H

#include "error.h"
#include <QVector>
#include <QUrl>
#include "SHARED_EXPORT.h"

class IProblem;
class ICompact;
class IVector;

class SHARED_EXPORT ISolver
{
public:
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    virtual int getId() const = 0;

    static ISolver* createSolver();

    virtual int setParams(IVector const* params) = 0;
    virtual int setParams(QString& str) = 0;
    virtual int setProblem(IProblem *ptr) = 0;
    virtual int solve() = 0;
    virtual int getSolution(IVector* &vec)const = 0;
    virtual int getQml(QUrl& qml) const = 0;
    virtual ~ISolver() = default;

protected:
    ISolver() = default;

private:
    /*non default copyable*/
    ISolver(const ISolver& other) = delete;
    void operator=( const ISolver& other) = delete;
};

#endif // ISOLVER_H
