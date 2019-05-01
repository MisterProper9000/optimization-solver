#ifndef IPROBLEM_H
#define IPROBLEM_H

#include "IVector.h"
#include "ICompact.h"

class /*Shared_Export???*/ IProblem
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

    /*non default copyable*/
    IProblem(const IProblem& other) = delete;
    void operator=( const IProblem& other) = delete;

    static IProblem* createProblem();

    virtual int goalFunction(IVector const* const args, IVector const* const params, double& res) = 0;
    virtual int goalFunctionByArgs(IVector const* const args, double& res) = 0;
    virtual int goalFunctionByParams(IVector const* const params, double& res) = 0;
    virtual size_t getArgsDim() = 0;
    virtual size_t getParamsDim() = 0;

    virtual int setCompact(ICompact const* ptr_comp) = 0;
    virtual derivativeGoalFunction(size_t order, size_t idx, /*enum DerivedType dr????*/, double& value, IVector const* ptr_args = nullptr, IVector const* ptr_params = nullptr) = 0;

    virtual ~IProblem() = default;
protected:
    IProblem() = default;

};

#endif // IPROBLEM_H
