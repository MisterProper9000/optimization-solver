#include <new>
#include <QFile>

#include <QFileInfo>
#include <cmath>
#include <QString>
#include <QStringList>

#include "ILog.h"
#include "IBrocker.h"
#include "ISolver.h"
#include "IProblem.h"
#include "ICompact.h"

namespace {

class ISolverImpl1 : public ISolver {

public:

    int getId() const;
    int setParams(IVector const* params);
    int setParams(QString& str);
    int setProblem(IProblem *ptr);
    int solve();
    int getSolution(IVector* &vec) const;
    int getQml(QUrl& qml) const;

    ISolverImpl1();

    ~ISolverImpl1();

private:
    int SolveByArguments();
    int SolveByParams();

    ICompact * m_compact;
    double eps;
    IVector * curr_solution;
    IProblem * m_problem;
    IVector *initial_args, *initial_params;
    bool byArgsFlag;
    size_t args_dim, params_dim;
    double curr_value;
};

int ISolverImpl1::getSolution(IVector* &res) const {
    if (!curr_solution) {
        ILog::report("ISolverImpl1::getSolution: solution doesn't exist");
        return ERR_ANY_OTHER;
    }
    IVector * tmp = curr_solution->clone();
    if (!tmp) {
        ILog::report("SolverImpl1::getSolution: ERR_MEMORY_ALLOCATION");
        return ERR_MEMORY_ALLOCATION;
    }
    if (res->getDim() != tmp->getDim()) {
        ILog::report("ISolverImpl1::getSolution: dimension mismatch");
        delete tmp;
        return ERR_DIMENSIONS_MISMATCH;
    }
    res = tmp;
    return ERR_OK;
}

int ISolverImpl1::SolveByArguments() {

    if (m_problem->setParams(initial_params) != ERR_OK) {
        ILog::report("ISolverImpl1::SolveByArguments : error with setting params to problem");
        return ERR_ANY_OTHER;
    }


    curr_solution = initial_args->clone();

    if (!curr_solution) {
        ILog::report("ISolverImpl1::SolveByArguments : ERR_MEMORY_ALLOCATION");
        return ERR_MEMORY_ALLOCATION;
    }

    if (m_problem->goalFunctionByArgs(curr_solution, curr_value) != ERR_OK) {
        ILog::report("ISolverImpl1::SolveByArguments : error by computing goal function");
        return ERR_ANY_OTHER;
    }

    while (true) {
        IVector * prev_solution = curr_solution;
        size_t argsDim;

        if (m_problem->getArgsDim(argsDim) != ERR_OK) {
            ILog::report("ISolverImpl1::SolveByArguments : can't get dim of args");
            return ERR_ANY_OTHER;
        }

        double *gradD = new (std::nothrow) double[argsDim];

        if (!gradD) {
            ILog::report("ISolverImpl1::SolveByArguments : ERR_MEMORY_ALLOCATION");
            return ERR_MEMORY_ALLOCATION;
        }

        for (size_t i = 0; i < argsDim; i++) {
            if (m_problem->derivativeGoalFunctionByArgs(1, i, IProblem::BY_ARGS, gradD[i], curr_solution) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByArguments: error with derivativeGoalFunctionByArgs");
                delete[] gradD;
                return ERR_ANY_OTHER;
            }
        }

        IVector *grad = IVector::createVector(argsDim, gradD);
        delete[] gradD;

        if (!grad) {
            ILog::report("ISolverImpl1::SolveByArguments: ERR_MEMORY_ALLOCATION");
            return ERR_MEMORY_ALLOCATION;
        }

        double grad_norm;


        if (grad->norm(IVector::NORM_INF, grad_norm) != ERR_OK) {
            ILog::report("ISolverImpl1::SolveByArguments: error with computing norm");
            delete grad;
            return ERR_ANY_OTHER;
        }

        if (grad_norm <= eps) {
            delete grad;
            return ERR_OK;
        }

        double alpha = 1, lambda = 0.8;

        while (true) {
            IVector *step = IVector::multiplyByScalar(grad, alpha);

            if (!step) {
                ILog::report("ISolverImpl1::SolveByArguments: error with multiplyByScalar");
                delete grad;
                return ERR_ANY_OTHER;
            }

            IVector *tmp = IVector::subtract(curr_solution, step);
            delete step;

            if (!tmp) {
                ILog::report("ISolverImpl1::SolveByArguments: error with subtract");
                delete grad;
                delete tmp;
                return ERR_ANY_OTHER;
            }

            IVector *new_solution;

            if (m_compact->getNearestNeighbor(tmp, new_solution) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByArguments: error with getNearestNeighbor");
                delete grad;
                delete tmp;
                return ERR_ANY_OTHER;
            }

            delete tmp;
            double new_value;

            if (m_problem->goalFunctionByArgs(new_solution, new_value) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByArguments: error with goalFunctionByArgs");
                delete grad;
                delete new_solution;
                return ERR_ANY_OTHER;
            }

            if (new_value <= curr_value) {
                curr_value = new_value;
                delete prev_solution;
                prev_solution = curr_solution;
                curr_solution = new_solution;
                break;
            } else {
                alpha *= lambda;
                delete new_solution;
            }

        }

        delete grad;

        bool compare_flag = false;

        if (curr_solution->eq(prev_solution, IVector::NORM_INF, compare_flag, eps) != ERR_OK) {
            ILog::report("ISolverImpl1::SolveByArguments: cannot compare two vectors\n");
            return ERR_ANY_OTHER;
        }
        if (compare_flag)
            break;
    }
    return ERR_OK;
}

int ISolverImpl1::SolveByParams() {

    if (m_problem->setArgs(initial_args) != ERR_OK) {
        ILog::report("ISolverImpl1::SolveByParams : error with setting args to problem");
        return ERR_ANY_OTHER;
    }


    curr_solution = initial_params->clone();

    if (!curr_solution) {
        ILog::report("ISolverImpl1::SolveByParams : ERR_MEMORY_ALLOCATION");
        return ERR_MEMORY_ALLOCATION;
    }

    if (m_problem->goalFunctionByParams(curr_solution, curr_value) != ERR_OK) {
        ILog::report("ISolverImpl1::SolveByParams : error by computing goal function");
        return ERR_ANY_OTHER;
    }

    while (true) {
        IVector * prev_solution = curr_solution;
        size_t paramsDim;

        if (m_problem->getParamsDim(paramsDim) != ERR_OK) {
            ILog::report("ISolverImpl1::SolveByParams : can't get dim of params");
            return ERR_ANY_OTHER;
        }

        double *gradD = new (std::nothrow) double[paramsDim];

        if (!gradD) {
            ILog::report("ISolverImpl1::SolveByParams : ERR_MEMORY_ALLOCATION");
            return ERR_MEMORY_ALLOCATION;
        }

        for (size_t i = 0; i < paramsDim; i++) {
            if (m_problem->derivativeGoalFunctionByParams(1, i, IProblem::BY_PARAMS, gradD[i], curr_solution) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByParams: error with derivativeGoalFunctionByParams");
                delete[] gradD;
                return ERR_ANY_OTHER;
            }
        }

        IVector *grad = IVector::createVector(paramsDim, gradD);
        delete[] gradD;

        if (!grad) {
            ILog::report("ISolverImpl1::SolveByParams: ERR_MEMORY_ALLOCATION");
            return ERR_MEMORY_ALLOCATION;
        }

        double grad_norm;


        if (grad->norm(IVector::NORM_INF, grad_norm) != ERR_OK) {
            ILog::report("ISolverImpl1::SolveByParams: error with computing norm");
            delete grad;
            return ERR_ANY_OTHER;
        }

        if (grad_norm <= eps) {
            delete grad;
            return ERR_OK;
        }

        double alpha = 1, lambda = 0.8;

        while (true) {
            IVector *step = IVector::multiplyByScalar(grad, alpha);

            if (!step) {
                ILog::report("ISolverImpl1::SolveByParams: error with multiplyByScalar");
                delete grad;
                return ERR_ANY_OTHER;
            }

            IVector *tmp = IVector::subtract(curr_solution, step);
            delete step;

            if (!tmp) {
                ILog::report("ISolverImpl1::SolveByParams: error with subtract");
                delete grad;
                delete tmp;
                return ERR_ANY_OTHER;
            }

            IVector *new_solution;

            if (m_compact->getNearestNeighbor(tmp, new_solution) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByParams: error with getNearestNeighbor");
                delete grad;
                delete tmp;
                return ERR_ANY_OTHER;
            }

            delete tmp;
            double new_value;

            if (m_problem->goalFunctionByParams(new_solution, new_value) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByParams: error with goalFunctionByParams");
                delete grad;
                delete new_solution;
                return ERR_ANY_OTHER;
            }

            if (new_value <= curr_value) {
                curr_value = new_value;
                delete prev_solution;
                prev_solution = curr_solution;
                curr_solution = new_solution;
                break;
            } else {
                alpha *= lambda;
                delete new_solution;
            }

        }

        delete grad;

        bool compare_flag = false;

        if (curr_solution->eq(prev_solution, IVector::NORM_INF, compare_flag, eps) != ERR_OK) {
            ILog::report("ISolverImpl1::SolveByParams: cannot compare two vectors\n");
            return ERR_ANY_OTHER;
        }
        if (compare_flag)
            break;
    }
    return ERR_OK;
}

int ISolverImpl1::solve() {
    if (!initial_args || !initial_params) {
        ILog::report("ISolverImpl1::Solve : invalid initial approximation");
        return ERR_WRONG_ARG;
    }

    if(!m_problem) {
        ILog::report("ISolverImpl1::Solve : invalid problem");
        return ERR_WRONG_ARG;
    }

    if (!m_compact) {
        ILog::report("ISolverImpl1::Solve : invalid compact");
        return ERR_WRONG_ARG;
    }


    if (byArgsFlag == true) {
        if (SolveByArguments() != ERR_OK) {
            ILog::report("ISolverImpl1::Solve : solve error");
            return ERR_ANY_OTHER;
        }
    }
    else {
        if (SolveByParams() != ERR_OK) {
            ILog::report("ISolverImpl1::Solve : solve error");
            return ERR_ANY_OTHER;
        }
    }
    return ERR_OK;
}

ISolverImpl1::ISolverImpl1():
    m_compact(NULL), curr_solution(NULL), m_problem(NULL), initial_args(NULL), initial_params(NULL)
{}

ISolverImpl1::~ISolverImpl1() {
    delete initial_args;
    delete initial_params;
    delete curr_solution;
    delete m_compact;
    m_problem = NULL;
}

int ISolverImpl1::getId() const {
    return ISolver::INTERFACE_0;
}

int ISolverImpl1::setProblem(IProblem * problem) {
    if (!problem) {
        ILog::report("ISolverImpl1::setProblem: Input parameter problem is nullptr");
        return ERR_WRONG_PROBLEM;
    }

    if (initial_args) {
        unsigned arg_dim;

        if (problem->getArgsDim(arg_dim) != ERR_OK) {
            ILog::report("ISolverImpl1::setProblem: Cannot check dimensions of arguments\n");
            return ERR_ANY_OTHER;
        }
        if (initial_args->getDim() != arg_dim) {
            ILog::report("ISolverImpl1::setProblem: Dimensions of arguments mismatch\n");
            return ERR_WRONG_PROBLEM;
        }
    }

    if (initial_params) {
        unsigned params_dim;

        if (problem->getParamsDim(params_dim) != ERR_OK) {
            ILog::report("ISolverImpl1::setProblem: Cannot check dimensions of parameters\n");
            return ERR_ANY_OTHER;
        }
        if (initial_params->getDim() != params_dim) {
            ILog::report("ISolverImpl1::setProblem: Dimensions of parameters mismatch\n");
            return ERR_WRONG_PROBLEM;
        }
    }
    m_problem = problem;
    return ERR_OK;
}

/*
  params:
  0 : dim of args
  1 : dim of params
  2 : epsilon
  3 : flag : solve by args or not
  ... all arguments
  ... all params
  ... begin vector of compact
  ... end vector of compact
  */

int ISolverImpl1::setParams(IVector const* params) {
    if (!params) {
        ILog::report("ISolverImpl1::setParams: params is null");
        return ERR_WRONG_ARG;
    }

    size_t length;
    const double * coords;

    // get dim of params and params in double format
    if (params->getCoordsPtr(length, coords) != ERR_OK) {
        ILog::report("ISolverImpl1::setParams: Cannot get coords from params");
        delete[]  coords;
        return ERR_ANY_OTHER;
    }

    if (length < 4) {
        ILog::report("ISolverImpl1::setParams: too less params");
        delete[]  coords;
        return ERR_WRONG_ARG;
    }

    size_t dimArgs = round(coords[0]);
    size_t dimParams = round(coords[1]);

    if (m_problem) {
        size_t problem_arg_dim;
        size_t problem_params_dim;

        if (m_problem->getArgsDim(problem_arg_dim) != ERR_OK) {
            ILog::report("ISolverImpl1::setParams: Cannot check dimensions of arguments\n");
            delete[] coords;
            return ERR_ANY_OTHER;
        }
        if (m_problem->getParamsDim(problem_params_dim) != ERR_OK) {
            ILog::report("ISolverImpl1::setParams: Cannot check dimensions of parameters\n");
            delete[] coords;
            return ERR_ANY_OTHER;
        }
        if (dimArgs != problem_arg_dim) {
            ILog::report("ISolverImpl1::setParams: Dimensions of arguments mismatch\n");
            delete[] coords;
            return ERR_WRONG_ARG;
        }
        if (dimParams != problem_params_dim) {
            ILog::report("ISolverImpl1::setParams: Dimensions of parameters mismatch\n");
            delete[] coords;
            return ERR_WRONG_ARG;
        }
    }

    double epsilon = coords[2];

    if (epsilon <= 0.0) {
        ILog::report("ISolverImpl1::setParams: epsilon <= 0");
        delete[] coords;
        return ERR_WRONG_ARG;
    }

    bool isByArgs = (bool) round(coords[3]);

    size_t compact_dim = isByArgs ? dimArgs : dimParams;

    if (length != 4 + dimArgs + dimParams + 2 * compact_dim) {
        ILog::report("ISolverImpl1::setParams: wrong dimension of params");
        delete[] coords;
        return ERR_WRONG_ARG;
    }

    const double* argsD = coords + 4;
    const double* paramsD = argsD + dimArgs;
    const double* beginD = paramsD + compact_dim;
    const double* endD = paramsD + 2 * compact_dim;

    IVector * args = IVector::createVector(dimArgs, argsD);
    if (!args) {
        ILog::report("ISolverImpl1::setParams: can't allocate memory for arguments");
        delete[] coords;
        return ERR_MEMORY_ALLOCATION;
    }

    IVector * paramV = IVector::createVector(dimParams, paramsD);
    if (!paramV) {
        delete args;
        delete[] coords;
        ILog::report("ISolverImpl1::setParams: can't allocate memory for parameters");
        return ERR_MEMORY_ALLOCATION;
    }

    IVector * begin = IVector::createVector(compact_dim, beginD);
    if (!begin) {
        delete args;
        delete paramV;
        delete[] coords;
        ILog::report("ISolverImpl1::setParams: can't allocate memory for begin of compact");
        return ERR_MEMORY_ALLOCATION;
    }

    IVector *end = IVector::createVector(compact_dim, endD);
    if (!end) {
        delete args;
        delete paramV;
        delete begin;
        delete[] coords;
        ILog::report("ISolverImpl1::setParams: can't allocate memory for end of compact");
        return ERR_MEMORY_ALLOCATION;
    }

    ICompact *compact = ICompact::createCompact(begin, end);
    if (!compact) {
        delete[] coords;
        delete args;
        delete paramV;
        delete begin;
        delete end;
        ILog::report("ISolverImpl1::setParams: can't allocate memory for compact");
        return ERR_MEMORY_ALLOCATION;
    }

    delete begin;
    delete end;

    delete initial_args;
    delete initial_params;
    delete m_compact;

    initial_args = args;
    initial_params = paramV;
    m_compact = compact;

    byArgsFlag = isByArgs;
    eps = epsilon;
    return ERR_OK;
}


int ISolverImpl1::setParams(QString & str) {
    QStringList coordsList = str.split(',');
    double *vecD = new double[coordsList.size()];

    for (int i = 0; i < coordsList.size(); i++) {
        bool res_convert = false;
        double elem = coordsList.at(i).toDouble(&res_convert);
        if (res_convert == false) {
            ILog::report("ISolverImpl1::setParams: can't convert str to double");
            return ERR_ANY_OTHER;
        }
        vecD[i] = elem;
    }

    IVector *vecParams = IVector::createVector(coordsList.size(), vecD); //?????
    return setParams(vecParams);
}

int ISolverImpl1::getQml(QUrl& qml) const {
    QFileInfo dll(qml.path());
    QString file = dll.absolutePath() + "/" + "solver1.qml";
    if (!QFile::exists(file))
        return ERR_ANY_OTHER;
    qml = QUrl::fromLocalFile(file);
    return ERR_OK;
}

//---------------------------BROCKER---------------------------------------------------------
class Brocker2: public virtual IBrocker{
public:
    int getId() const;
    bool canCastTo(Type type) const;
    void* getInterfaceImpl(Type type) const;
    int release();

    Brocker2(ISolverImpl1 *solver);

    ~Brocker2();

    //Brocker2();

private:
    ISolverImpl1 * m_solver;
};

int Brocker2::getId() const{
    return INTERFACE_0;
}

Brocker2::Brocker2(ISolverImpl1 *solver): m_solver(solver) {}

Brocker2::~Brocker2() {
    delete m_solver;
}

bool Brocker2::canCastTo(Type type) const{
    return type == IBrocker::SOLVER ? true : false;
}

void* Brocker2::getInterfaceImpl(Type type) const{
    return type == IBrocker::SOLVER ? m_solver : NULL;
}

int Brocker2::release(){
    delete this->m_solver;
    delete this;
    return ERR_OK;
}

}
