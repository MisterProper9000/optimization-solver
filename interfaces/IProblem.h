#ifndef IPROBLEM_H
#define IPROBLEM_H

#include "error.h"
#include "IVector.h"
#include "ICompact.h"
#include "SHARED_EXPORT.h"

class SHARED_EXPORT IProblem
{
public:
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    enum DerivedType
    {
        BY_ARGS,
        BY_PARAMS,
        DIMENSION_DERIVED
    };

    virtual int getId() const = 0;

    static IProblem* createProblem();

    virtual int goalFunction(IVector const* args, IVector const* params, double& res) = 0;
    virtual int goalFunctionByArgs(IVector const*  args, double& res) = 0;
    virtual int goalFunctionByParams(IVector const*  params, double& res) = 0;
    virtual int getArgsDim(size_t& dim) = 0;
    virtual int getParamsDim(size_t& dim) = 0;

    virtual int setCompact(ICompact const* ptr_comp) = 0;
    virtual derivativeGoalFunction(size_t order, size_t idx, enum DerivedType dr, double& value, IVector const* ptr_args = nullptr, IVector const* ptr_params = nullptr) = 0;

    virtual ~IProblem() = default;
protected:
    IProblem() = default;

private:
    /*non default copyable*/
    IProblem(const IProblem& other) = delete;
    void operator=( const IProblem& other) = delete;

};

#endif // IPROBLEM_H
