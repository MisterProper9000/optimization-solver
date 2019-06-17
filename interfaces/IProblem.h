#ifndef IPROBLEM_H
#define IPROBLEM_H

#include "IVector.h"
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

    virtual int goalFunction(IVector const* args, IVector const* params, double& res) const = 0;
    virtual int goalFunctionByArgs(IVector const*  args, double& res) const = 0;
    virtual int goalFunctionByParams(IVector const*  params, double& res) const = 0;
    virtual int getArgsDim(size_t& dim) const = 0;
    virtual int getParamsDim(size_t& dim) const = 0;

    virtual int setParams(IVector const* params) = 0;
    virtual int setArgs(IVector const* args) = 0;

    virtual int derivativeGoalFunction(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args, IVector const* params) const = 0;
    virtual int derivativeGoalFunctionByArgs(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args) const = 0;
    virtual int derivativeGoalFunctionByParams(size_t order, size_t idx, DerivedType dr, double& value, IVector const* params) const = 0;

protected:
    virtual ~IProblem(){}

    IProblem(){}

private:
    /*non default copyable*/
    IProblem(const IProblem& other) = delete;
    void operator=(const IProblem& other) = delete;

};

#endif // IPROBLEM_H
