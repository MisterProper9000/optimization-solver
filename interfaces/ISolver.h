#ifndef ISOLVER_H
#define ISOLVER_H

#include <QVector>
#include <QUrl>

class IProblem;
class ICompact;
class IVector;

class ISolver
{
public:

    //need to move ReturnCode from IProblem to its own namespace??
    enum ReturnCode
    {
        ERR_OK = 0,
        ERR_DIFFERENT_DIMENSIONS,
        ERR_MEMORY_ALLOCATION,
        ERR_NORM_NOT_DEFINED,
        ERR_OUT_OF_RANGE,
        ERR_VARIABLES_NUMBER_MISMATCH,
        NO_SOLUTION,
    };

    /*non default copyable*/
    ISolver(const ISolver& other) = delete;
    void operator=( const ISolver& other) = delete;

    static ISolver* createSolver();

    virtual int setParams(IVector const* const params) = 0;
    virtual int setParams(QString& str) = 0;
    virtual int setProblem(IProblem *ptr) = 0;
    virtual int setParams(IProblem *ptr, ICompact* cptr) = 0;
    virtual int solve() = 0;
    virtual int getRes(double &res) const = 0;
    virtual int getSolution(IVector* &vec)const = 0;
    virtual int SetPrecision(double pr) = 0;
    virtual int getQml(QUrl& qml) = 0;
    virtual ~ISolver() = default;

protected:
    ISolver() = default;
};

#endif // ISOLVER_H
