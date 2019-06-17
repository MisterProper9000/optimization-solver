#include "IProblem.h"
#include <math.h>
namespace{
class IProblemImpl: public IProblem {
public:
    virtual int getId() const;

    virtual int goalFunction(IVector const* args, IVector const* params, double& res) const;
    virtual int goalFunctionByArgs(IVector const*  args, double& res) const;
    virtual int goalFunctionByParams(IVector const*  params, double& res) const;
    virtual int getArgsDim(size_t& dim) const;
    virtual int getParamsDim(size_t& dim) const;

    virtual int setParams(IVector const* params);
    virtual int setArgs(IVector const* args);

    virtual int derivativeGoalFunction(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args, IVector const* params) const;
    virtual int derivativeGoalFunctionByArgs(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args) const;
    virtual int derivativeGoalFunctionByParams(size_t order, size_t idx, DerivedType dr, double& value, IVector const* params) const;

protected:
    virtual ~IProblemImpl() {}

    IProblemImpl(){}

private:
    /*non default copyable*/
    IProblemImpl(const IProblemImpl& other) = delete;
    void operator=(const IProblemImpl& other) = delete;
    IVector* m_params;
    IVector* m_args;
};
}

int IProblemImpl::goalFunction(IVector const* args, IVector const* params, double& res) const {
    if (args && (args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    res = sin(x);
    return ERR_OK;
}

int IProblemImpl::goalFunctionByArgs(IVector const* args, double& res) const {
    if (args && (args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    res = sin(x);
    return ERR_OK;
}

int IProblemImpl::goalFunctionByParams(IVector const* params, double& res) const {
    if (m_args && (m_args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (m_args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    res = sin(x);
    return ERR_OK;
}

int IProblemImpl::derivativeGoalFunction(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args, IVector const* params) const {
    if (args && (args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    if (DerivedType == BY_PARAMS){
        value = 0;
        return ERR_OK;
    }
    if (DerivedType != BY_ARGS){
        return ERR_WRONG_ARG;
    }
    if (idx != 0)
        return ERR_WRONG_ARG;
    switch (order % 4){
    case 0:
        value = sin(x);
        break;
    case 1:
        value = cos(x);
        break;
    case 2:
        value = -sin(x);
        break;
    case 3:
        value = -cos(x);
        break;
    }
    return ERR_OK;
}

int IProblemImpl::derivativeGoalFunctionByArgs(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args) const{
    return derivativeGoalFunction(order, idx, dr, value, args, m_params);
}

int IProblemImpl::derivativeGoalFunctionByParams(size_t order, size_t idx, DerivedType dr, double& value, IVector const* params) const{
    return derivativeGoalFunction(order, idx, dr, value, m_args, params);
}

int IProblemImpl:: getId(){
    return INTERFACE_0;
}

int IProblemImpl::setParams(IVector const* params){
    m_params = params->clone();
    if (m_params == nullptr)
        return ERR_ANY_OTHER;// maybe MEMORY_ALLOCATION
    return ERR_OK;
}
int IProblemImpl::setArgs(IVector const* args){
    m_args = args->clone();
    if (m_args == nullptr)
        return ERR_ANY_OTHER;// maybe MEMORY_ALLOCATION
    return ERR_OK;
}

int IProblemImpl::getArgsDim(size_t& dim) const{
    if (!m_args)
        return ERR_ANY_OTHER;
    dim = m_args->getDim();
    return ERR_OK;
}
int IProblemImpl::getParamsDim(size_t& dim) const{
    if (!m_params)
        return ERR_ANY_OTHER;
    dim = m_params->getDim();
    return ERR_OK;
}

