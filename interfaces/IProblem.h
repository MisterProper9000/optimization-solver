#ifndef IPROBLEM_H
#define IPROBLEM_H

#include "IVector.h"
#include "ICompact.h"

class IProblem
{
public:
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

    static IProblem* createProblem();

    virtual int goalFunction(IVector const* const args, IVector const* const params, double& res) = 0;
    virtual int goalFunctionByArgs(IVector const* const args, double& res) = 0;
    virtual int goalFunctionByParams(IVector const* const params, double& res) = 0;
    virtual size_t getArgsDim() = 0;
    virtual size_t getParamsDim() = 0;

    virtual int derivativeByArgs(size_t index, IVector const* const args, IVector const* const params, double& res) = 0;
    virtual int derivativeByParams(size_t index, IVector const* const, IVector const* const params, double& res) = 0;

    virtual bool validateArgs(QSharedPointer<ICompact> c) const = 0;
    virtual bool validateParams(QSharedPointer<ICompact> c) const = 0;

protected:

};

#endif // IPROBLEM_H
