#include <new>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <cmath>
#include <QRegExp>
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
   int ISolverImpl1::SolveByArguments();
   int ISolverImpl1::SolveByParams();

   ICompact * m_compact;
   double eps;
   IVector * curr_solution;
   IProblem * m_problem;
   IVector * initial_args, initial_params;
   bool byArgsFlag;
   size_t args_dim, params_dim;
   double curr_value;
};

class Brocker2 : public IBrocker {

public:

   int getId() const;

   bool canCastTo(Type type) const;
   void* getInterfaceImpl(Type type) const;

   int release();

   Brocker2(ISolverImpl1 *solver);
   ~Brocker2();

private:

    ISolverImpl1 *solver;

};

}

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
    // зафиксируем в задаче параметры
    if (m_problem->setParams(initial_params) != ERR_OK) {
        ILog::report("ISolverImpl1::SolveByArguments : error with setting params to problem");
        return ERR_ANY_OTHER;
    }

    // копируем  начальное приближение
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

        // проверка: вдруг точка уже оптимум
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
            IVector step = IVector::multiplyByScalar(grad, alpha);

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
    // зафиксируем в задаче аргументы
    if (m_problem->setArgs(initial_args) != ERR_OK) {
        ILog::report("ISolverImpl1::SolveByParams : error with setting args to problem");
        return ERR_ANY_OTHER;
    }

    // копируем  начальные параметры
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

        for (size_t i = 0; i < argsDim; i++) {
            if (m_problem->derivativeGoalFunctionByParams(1, i, IProblem::BY_PARAMS, gradD[i], curr_solution) != ERR_OK) {
                ILog::report("ISolverImpl1::SolveByParams: error with derivativeGoalFunctionByParams");
                delete[] gradD;
                return ERR_ANY_OTHER;
            }
        }

        IVector *grad = IVector::createVector(argsDim, gradD);
        delete[] gradD;

        if (!grad) {
            ILog::report("ISolverImpl1::SolveByParams: ERR_MEMORY_ALLOCATION");
            return ERR_MEMORY_ALLOCATION;
        }

        double grad_norm;

        // проверка: вдруг точка уже оптимум
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
            IVector step = IVector::multiplyByScalar(grad, alpha);

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
    initial_args(NULL), initial_params(NULL), m_compact(NULL), curr_solution(NULL), m_problem(NULL)
{}

ISolverImpl1::~ISolverImpl1() {
    delete initial_args;
    delete initial_params;
    delete m_problem;
    delete curr_solution;
    delete m_compact;
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

int ISolverImpl1::setParams(IVector const* params) {
    if (!params) {
        ILog::report("ISolver.setParams: Input parameter ptr is nullptr\n");
        return ERR_WRONG_ARG;
    }
    unsigned int dim, dimArgs, dimParams, tmp;
    const double * coords;
    bool solveByArg;
    double epsilon;
    IVector * args, * param, * begin, * end;
    ICompact * compact;
    if (params->getCoordsPtr(dim, coords) != ERR_OK) {
        ILog::report("ISolver.setParams: Cannot get coords from params\n");
        return ERR_ANY_OTHER;
    }
    if (dim < 4) {
        ILog::report("ISolver.setParams: Dimension of params less than 4\n");
        return ERR_WRONG_ARG;
    }
    if (_problem) {
        if (_problem->getArgsDim(dimArgs) != ERR_OK) {
            ILog::report("ISolver.setParams: Cannot check dimensions of arguments\n");
            return ERR_ANY_OTHER;
        }
        if (_problem->getParamsDim(dimParams) != ERR_OK) {
            ILog::report("ISolver.setParams: Cannot check dimensions of parameters\n");
            return ERR_ANY_OTHER;
        }
        if (round(coords[0]) != dimArgs) {
            ILog::report("ISolver.setParams: Dimensions of arguments mismatch\n");
            return ERR_WRONG_ARG;
        }
        if (round(coords[1]) != dimParams) {
            ILog::report("ISolver.setParams: Dimensions of parameters mismatch\n");
            return ERR_WRONG_ARG;
        }
    } else {
        dimArgs = round(coords[0]);
        dimParams = round(coords[1]);
    }
    epsilon = coords[2];
    if (epsilon <= 0) {
        ILog::report("ISolver.setParams: Epsilon is negative\n");
        return ERR_WRONG_ARG;
    }
    tmp = round(coords[3]);
    if (tmp != 1 || tmp != 0) {
        ILog::report("ISolver.setParams: Wrong flag for solve by arguments\n");
        return ERR_WRONG_ARG;
    }
    solveByArg = static_cast<bool>(tmp);
    tmp = dimArgs + dimParams + 4;
    if ((solveByArg && dim != tmp + 2 * dimArgs) || (!solveByArg && dim != tmp + 2 * dimParams)) {
        ILog::report("ISolver.setParams: Dimension of params is wrong\n");
        return ERR_WRONG_PROBLEM;
    }
    dim = solveByArg ? dimArgs : dimParams;
    args = IVector::createVector(dimArgs, (coords + 4));
    if (!args) {
        ILog::report("ISolver.setParams: Canntot alloc memory for arguments\n");
        return ERR_MEMORY_ALLOCATION;
    }
    param = IVector::createVector(dimParams, (coords + 4 + dimArgs));
    if (!param) {
        delete args;
        ILog::report("ISolver.setParams: Canntot alloc memory for parameters\n");
        return ERR_MEMORY_ALLOCATION;
    }
    begin = IVector::createVector(dim, (coords + tmp));
    if (!begin) {
        delete args;
        delete params;
        ILog::report("ISolver.setParams: Canntot alloc memory for begin of compact\n");
        return ERR_MEMORY_ALLOCATION;
    }
    end = IVector::createVector(dim, (coords + tmp + dim));
    if (!end) {
        delete args;
        delete params;
        delete begin;
        ILog::report("ISolver.setParams: Canntot alloc memory for end of compact\n");
        return ERR_MEMORY_ALLOCATION;
    }
    compact = ICompact::createCompact(begin, end);
    if (!compact) {
        delete args;
        delete params;
        delete begin;
        delete end;
        ILog::report("ISolver.setParams: Canntot alloc memory for compact\n");
        return ERR_MEMORY_ALLOCATION;
    }
    delete begin;
    delete end;
    delete _args;
    delete _params;
    delete _compact;
    _args = args;
    _params = param;
    _compact = compact;
    solveByArgs = solveByArg;
    eps = epsilon;
    return ERR_OK;
}

int ISolverImpl1::setParams(QString & str) {
    unsigned int dim, dimArgs, dimArgs1, dimParams, dimParams1, tmp2;
    QString tmp;
    double * coords;
    bool solveByArg, ok;
    double epsilon;
    IVector * args, * param, * begin, * end;
    ICompact * compact;
    QStringList preparams = str.split(" "), params, tmpList;
    if (preparams.count() < 4) {
        ILog::report("ISolver.setParams: Dimension of params less than 4\n");
        return ERR_WRONG_ARG;
    }
    foreach (QString s, preparams) {
        tmpList = s.split(":");
        if (tmpList.count() != 2) {
            ILog::report("ISolver.setParams: Wrong count of params\n");
            return ERR_WRONG_ARG;
        }
        params.append(s.split(":").at(1));
    }

    if (_problem) {
        if (_problem->getArgsDim(dimArgs) != ERR_OK) {
            ILog::report("ISolver.setParams: Cannot check dimensions of arguments\n");
            return ERR_ANY_OTHER;
        }
        if (_problem->getParamsDim(dimParams) != ERR_OK) {
            ILog::report("ISolver.setParams: Cannot check dimensions of parameters\n");
            return ERR_ANY_OTHER;
        }
        dimArgs1 = params.at(0).toUInt(&ok);
        if (!ok) {
            ILog::report("ISolver.setParams: Cannot get dimension of arguments\n");
            return ERR_WRONG_ARG;
        }
        if (!ok || dimArgs1 != dimArgs) {
            ILog::report("ISolver.setParams: Dimensions of arguments mismatch\n");
            return ERR_WRONG_ARG;
        }
        dimParams1 = params.at(1).toUInt(&ok);
        if (!ok) {
            ILog::report("ISolver.setParams: Cannot get dimension of parameters\n");
            return ERR_WRONG_ARG;
        }
        if (dimParams1 != dimParams) {
            ILog::report("ISolver.setParams: Dimensions of parameters mismatch\n");
            return ERR_WRONG_ARG;
        }
    } else {
        dimArgs = params.at(0).toUInt(&ok);
        if (!ok) {
            ILog::report("ISolver.setParams: Cannot get dimension of arguments\n");
            return ERR_WRONG_ARG;
        }
        dimParams = params.at(1).toUInt(&ok);
        if (!ok) {
            ILog::report("ISolver.setParams: Cannot get dimension of parameters\n");
            return ERR_WRONG_ARG;
        }
    }
    epsilon = params.at(2).toDouble(&ok);
    if (!ok) {
        ILog::report("ISolver.setParams: Cannot get epsilon\n");
        return ERR_WRONG_ARG;
    }
    if (epsilon <= 0) {
        ILog::report("ISolver.setParams: Epsilon is negative\n");
        return ERR_WRONG_ARG;
    }
    tmp = params.at(3);
    if (tmp.length() == 0) {
        ILog::report("ISolver.setParams: Wrong string for solve by arguments or parameters\n");
        return ERR_WRONG_ARG;
    } else if (QRegExp("[Aa][Rr][Gg][Ss]").exactMatch(tmp)) {
        solveByArg = true;
    } else if (QRegExp("[Pp][Aa][Rr][Aa][Mm][Ss]").exactMatch(tmp)) {
        solveByArg = false;
    } else {
        ILog::report("ISolver.setParams: Wrong string for solve by arguments or parameters\n");
        return ERR_WRONG_ARG;
    }
    if (static_cast<unsigned>(params.count()) != dimArgs + dimParams + 4 + 2 * std::max(dimArgs, dimParams)) {
        ILog::report("ISolver.setParams: Count of params is wrong\n");
        return ERR_WRONG_PROBLEM;
    }
    dim = solveByArg ? dimArgs : dimParams;
    coords = new (std::nothrow) double[std::max(dimArgs, dimParams)];
    if (!coords) {
        ILog::report("ISolver.setParams: Canntot alloc memory\n");
        return ERR_MEMORY_ALLOCATION;
    }
    tmp2 = 4;
    for (unsigned i = 0; i < dimArgs; ++i) {
        coords[i] = params.at(i + tmp2).toDouble(&ok);
        if (!ok) {
            delete coords;
            ILog::report("ISolver.setParams: Cannot get args\n");
            return ERR_ANY_OTHER;
        }
    }
    args = IVector::createVector(dimArgs, (coords));
    if (!args) {
        delete coords;
        ILog::report("ISolver.setParams: Canntot alloc memory for arguments\n");
        return ERR_MEMORY_ALLOCATION;
    }
    tmp2 += dimArgs;
    for (unsigned i = 0; i < dimParams; ++i) {
        coords[i] = params.at(i + tmp2).toDouble(&ok);
        if (!ok) {
            delete coords;
            delete args;
            ILog::report("ISolver.setParams: Cannot get params\n");
            return ERR_ANY_OTHER;
        }
    }
    param = IVector::createVector(dimParams, (coords));
    if (!param) {
        delete coords;
        delete args;
        ILog::report("ISolver.setParams: Canntot alloc memory for parameters\n");
        return ERR_MEMORY_ALLOCATION;
    }
    tmp2 += dimParams;
    for (unsigned i = 0; i < dim; ++i) {
        coords[i] = params.at(i + tmp2).toDouble(&ok);
        if (!ok) {
            delete coords;
            delete args;
            delete param;
            ILog::report("ISolver.setParams: Cannot get begin of compact\n");
            return ERR_ANY_OTHER;
        }
    }
    begin = IVector::createVector(dim, (coords));
    if (!begin) {
        delete coords;
        delete args;
        delete param;
        ILog::report("ISolver.setParams: Canntot alloc memory for begin of compact\n");
        return ERR_MEMORY_ALLOCATION;
    }
    tmp2 += std::max(dimArgs, dimParams);
    for (unsigned i = 0; i < dim; ++i) {
        coords[i] = params.at(i + tmp2).toDouble(&ok);
        if (!ok) {
            delete coords;
            delete args;
            delete param;
            delete begin;
            ILog::report("ISolver.setParams: Cannot get end of compact\n");
            return ERR_ANY_OTHER;
        }
    }
    end = IVector::createVector(dim, (coords));
    if (!end) {
        delete coords;
        delete args;
        delete param;
        delete begin;
        ILog::report("ISolver.setParams: Canntot alloc memory for end of compact\n");
        return ERR_MEMORY_ALLOCATION;
    }
    delete coords;
    compact = ICompact::createCompact(begin, end);
    if (!compact) {
        delete args;
        delete param;
        delete begin;
        delete end;
        ILog::report("ISolver.setParams: Canntot alloc memory for compact\n");
        return ERR_MEMORY_ALLOCATION;
    }
    delete begin;
    delete end;
    delete _args;
    delete _params;
    delete _compact;
    _args = args;
    _params = param;
    _compact = compact;
    solveByArgs = solveByArg;
    eps = epsilon;
    return ERR_OK;
}

int Brocker2::getId() const {
    return IBrocker::INTERFACE_0;
}

int ISolverImpl1::getQml(QUrl& qml) const {
    QFileInfo dll(qml.path());
    QString file = dll.absolutePath() + "/" + "ISolverImpl1.qml";
    if (!QFile::exists(file))
        return ERR_ANY_OTHER;
    qml = QUrl::fromLocalFile(file);
    return ERR_OK;
}

bool Brocker2::canCastTo(Type type) const {
    switch (type) {
    case IBrocker::SOLVER:
        return true;
    default:
        return false;
    }
}

void* Brocker2::getInterfaceImpl(Type type) const {
    switch (type) {
    case IBrocker::SOLVER:
        return _solver;
    default:
        return NULL;
    }
}

int Brocker2::release() {
    delete this;

    return ERR_OK;
}

Brocker2::Brocker2(ISolverImpl1 *solver):
    _solver(solver)
{}

Brocker2::~Brocker2() {
    delete _solver;
}

extern "C" {
SHARED_EXPORT void* getBrocker() {
    ISolverImpl1 *solver = new (std::nothrow) ISolverImpl1();

    if (!solver) {
        ILog::report("getBrocker: not enough memory\n");
        return NULL;
    }

    Brocker2 *brocker = new (std::nothrow) Brocker2(solver);

    if (!brocker) {
        ILog::report("getBrocker: not enough memory\n");
        delete solver;
        return NULL;
    }

    return brocker;
}
}
