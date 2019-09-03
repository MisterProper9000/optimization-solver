#include "IProblem.h"
#include "IBrocker.h"
#include "ISolver.h"
#include "ILog.h"

#include <math.h>
namespace{
class IProblemImpl1: public IProblem {
public:
    int getId() const;

    int goalFunction(IVector const* args, IVector const* params, double& res) const;
    int goalFunctionByArgs(IVector const*  args, double& res) const;
    int goalFunctionByParams(IVector const*  params, double& res) const;
    int getArgsDim(size_t& dim) const;
    int getParamsDim(size_t& dim) const;

    int setParams(IVector const* params);
    int setArgs(IVector const* args);

    int derivativeGoalFunction(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args, IVector const* params) const;
    int derivativeGoalFunctionByArgs(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args) const;
    int derivativeGoalFunctionByParams(size_t order, size_t idx, DerivedType dr, double& value, IVector const* params) const;

    ~IProblemImpl1() {}

    IProblemImpl1(){}

private:

    IVector* m_params;
    IVector* m_args;
};
}

int IProblemImpl1::goalFunction(IVector const* args, IVector const* params, double& res) const {
    if (args && (args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    res = sin(x);
    return ERR_OK;
}

int IProblemImpl1::goalFunctionByArgs(IVector const* args, double& res) const {
    if (args && (args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    res = sin(x);
    return ERR_OK;
}

int IProblemImpl1::goalFunctionByParams(IVector const* params, double& res) const {
    if (m_args && (m_args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (m_args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    res = sin(x);
    return ERR_OK;
}

int IProblemImpl1::derivativeGoalFunction(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args, IVector const* params) const {
    if (args && (args->getDim() != 1))
        return ERR_DIMENSIONS_MISMATCH;
    double x;
    if (args->getCoord(0, x) != ERR_OK)
        return ERR_ANY_OTHER;
    if (dr == BY_PARAMS){
        value = 0;
        return ERR_OK;
    }
    if (dr != BY_ARGS){
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

int IProblemImpl1::derivativeGoalFunctionByArgs(size_t order, size_t idx, DerivedType dr, double& value, IVector const* args) const{
    return derivativeGoalFunction(order, idx, dr, value, args, m_params);
}

int IProblemImpl1::derivativeGoalFunctionByParams(size_t order, size_t idx, DerivedType dr, double& value, IVector const* params) const{
    return derivativeGoalFunction(order, idx, dr, value, m_args, params);
}

int IProblemImpl1::getId() const{
    return INTERFACE_0;
}

int IProblemImpl1::setParams(IVector const* params){
    m_params = params->clone();
    if (m_params == NULL)
        return ERR_MEMORY_ALLOCATION;
    return ERR_OK;
}
int IProblemImpl1::setArgs(IVector const* args){
    m_args = args->clone();
    if (m_args == NULL)
        return ERR_MEMORY_ALLOCATION;
    return ERR_OK;
}

int IProblemImpl1::getArgsDim(size_t& dim) const{
    if (!m_args)
        return ERR_ANY_OTHER;
    dim = m_args->getDim();
    return ERR_OK;
}
int IProblemImpl1::getParamsDim(size_t& dim) const{
    if (!m_params)
        return ERR_ANY_OTHER;
    dim = m_params->getDim();
    return ERR_OK;
}

//---------------------------BROCKER---------------------------------------------------------
class Brocker1: public virtual IBrocker{
public:
    int getId() const;
    bool canCastTo(Type type) const;
    void* getInterfaceImpl(Type type) const;
    int release();

    Brocker1(IProblemImpl1 *problem);

    ~Brocker1();

    Brocker1();

private:
   IProblemImpl1 * m_problem;
};

int Brocker1::getId() const{
   return INTERFACE_0;
}

Brocker1::Brocker1(IProblemImpl1 *problem): m_problem(problem) {}

Brocker1::~Brocker1() {
    delete m_problem;
}

bool Brocker1::canCastTo(Type type) const{
   return type == IBrocker::SOLVER ? true : false;
}

void* Brocker1::getInterfaceImpl(Type type) const{
   return type == IBrocker::SOLVER ? m_problem : NULL;
}

int Brocker1::release(){
    delete this->m_problem;
    delete this;
    return ERR_OK;
}

extern "C" {
    /*SHARED_EXPORT*/ void* getBrocker() {
        IProblemImpl1 *problem = new IProblemImpl1();

        if (!problem) {
            ILog::report("getBrocker: not enough memory\n");
            return NULL;
        }

        Brocker1 *brocker = new Brocker1(problem);

        if (!brocker) {
            ILog::report("getBrocker: not enough memory\n");
            delete problem;
            return NULL;
        }

        return brocker;
    }
}
