#ifndef ISOLVER_H
#define ISOLVER_H

#include <QVector>

class IProblem;
class ICompact;
class IVector;

class ISolver
{
public:

    /*non default copyable*/
    ISolver(const ISolver& other) = delete;
    void operator=( const ISolver& other) = delete;

    static ISolver* createSolver();
    virtual int SetParamsQml(QString str) = 0;
    virtual int SetProblem(IProblem *ptr) = 0;
    virtual int SetParams(IProblem *ptr, ICompact* cptr) = 0;
    virtual int run() = 0;
    virtual int getRes(double &res)const = 0;
    virtual int getOptimum(IVector* &vec)const = 0;
    virtual int SetPrecision(double pr) = 0;

    virtual ~ISolver() = default;

protected:
    ISolver() = default;
};

#endif // ISOLVER_H
