#include <qglobal.h>
#include "IVector.h"
#include "QVector"
#include <math.h>
#include <iostream>
#include <cfloat>
#include "ILog.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))
#define SIGN(x) (x >= 0) ? ((x>0)?(1):(0)):(-1)

namespace {
    class IVectorImpl: public virtual IVector
    {
    public:
        int getId() const;

        static IVector* createVector(unsigned int size, const double *vals);

        /*operations*/
        virtual int add(IVector const* const right);
        virtual int subtract(IVector const* const right);
        virtual int multiplyByScalar(double scalar);
        virtual int dotProduct(IVector const* const right, double& res) const;

        /*comparators*/
        virtual int gt(IVector const* const right, NormType type, bool& result) const;
        virtual int lt(IVector const* const right, NormType type, bool& result) const;
        virtual int eq(IVector const* const right, NormType type, bool& result, double precision) const;

        /*utils*/
        virtual unsigned int getDim() const;
        virtual int norm(NormType type, double& res) const;
        virtual int setCoord(unsigned int index, double elem);
        virtual int setAllCoords(unsigned int dim, double* coords);
        virtual int setAllCoords(IVector const* const other);
        virtual int getCoord(unsigned int index, double & elem) const;
        virtual int getCoordsPtr(unsigned int & dim, double const*& elem) const;
        virtual IVector* clone() const;

        /*dtor*/
        virtual ~IVectorImpl();

    protected:
        IVectorImpl() = default;
        IVectorImpl(unsigned int size, double* vals);
        double* m_vec;
        unsigned int m_dim;

    private:
        /*non default copyable*/
        IVectorImpl(const IVectorImpl& other) = delete;
        void operator=( const IVectorImpl& other) = delete;
    };
}

// return interface implementation ID
int IVectorImpl::getId() const
{
    return DIMENSION_INTERFACE_IMPL;
}

// @return pointer to IVector instance
IVector* IVector::createVector(unsigned int size, double const* vals)
{
    if (!vals)
    {
        ILog::report("IVector::createVector: Vector init with nullptr, expected double pointer");
        return 0;
    }
    if (size == 0)
    {
        ILog::report("IVector::createVector: Vector can't be zero-dimensional");
        return 0;
    }

    return IVectorImpl::createVector(size, vals);
}

IVector* IVectorImpl::createVector(unsigned int size, const double *vals)
{
    double* try_vec = new double[size];
    if(try_vec)
    {
        for(int i = 0; i < size; i++)
        {
            try_vec[i] = vals[i];
        }
        return new IVectorImpl(size, try_vec);
    }
    else
    {
        ILog::report("IVector::createVector: not enough memory");
        return 0;
    }
}

// @param size - legth of data array
// @param vals - data array
// creates VectorImpl
IVectorImpl::IVectorImpl(unsigned int size, double* vals): m_dim(size), m_vec(vals){}

/*operations*/

// @return error code
// @param right - right operand
// coordinatewise addition: this + right
int IVectorImpl::add(IVector const* const right)
{
    if (!right)
    {
        ILog::report("IVector.add: right operand of addition is nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != right->getDim())
    {
        ILog::report("IVector.add: dimension mismatch in vector's addition");
        return ERR_DIMENSIONS_MISMATCH;
    }
    int lastError = ERR_OK;
    const double *p_tmp;

    double* this_tmp_vec = new double[m_dim];
    if(!this_tmp_vec)
    {
        ILog::report("IVector.add: not enough memory");
        return ERR_MEMORY_ALLOCATION;
    }

    unsigned int r_dim;
    if ((lastError = right->getCoordsPtr(r_dim, p_tmp)) != ERR_OK)
    {
        delete[] this_tmp_vec;
        return lastError;
    }

    for (unsigned int i = 0; i < m_dim; i++)
    {
        if (isnan(m_vec[i] + p_tmp[i]))
        {
            ILog::report("IVector.add: double overfulls");
            delete[] this_tmp_vec;
            return ERR_OVERFULL;
        }
        else
        {
            this_tmp_vec[i] =  m_vec[i] + p_tmp[i];
        }
    }

    delete[] m_vec;
    m_vec = this_tmp_vec;

    return lastError;
}

// @return error code
// @param right - right operand
// coordinatewise subtraction: this - right
int IVectorImpl::subtract(IVector const* const right)
{
    if (!right)
    {
        ILog::report("IVector.subtract: right operand of subtraction is nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != right->getDim())
    {
        ILog::report("IVector.subtract: Dimension mismatch in vector's subtraction");
        return ERR_DIMENSIONS_MISMATCH;
    }

    double* this_tmp_vec = new double[m_dim];
    if(!this_tmp_vec)
    {
        ILog::report("IVector.add: not enough memory");
        return ERR_MEMORY_ALLOCATION;
    }

    int lastError = ERR_OK;
    const double *p_tmp;
    unsigned int r_dim;
    if ((lastError = right->getCoordsPtr(r_dim, p_tmp)) != ERR_OK)
    {
        delete[] this_tmp_vec;
        return lastError;
    }

    for (unsigned int i = 0; i < m_dim; i++)
    {
        if (isnan(m_vec[i] - p_tmp[i]))
        {
            ILog::report("IVector.add: double overfulls");
            delete[] this_tmp_vec;
            return ERR_OVERFULL;
        }
        else
            this_tmp_vec[i] = m_vec[i] - p_tmp[i];
    }

    delete[] m_vec;
    m_vec = this_tmp_vec;

    return lastError;
}

// @return error code
// @param scalar - scalar multiplier
// coordinatewise multiplication: this * scalar
int IVectorImpl::multiplyByScalar(double scalar)
{
    double* this_tmp_vec = new double[m_dim];
    if(!this_tmp_vec)
    {
        ILog::report("IVector.add: not enough memory");
        return ERR_MEMORY_ALLOCATION;
    }

    for (unsigned int i = 0; i < m_dim; i++)
    {
        if(isnan(m_vec[i]*scalar))
        {
            ILog::report("IVector.multiplyByScalar: double overfulls");
            delete[] this_tmp_vec;
            return ERR_OVERFULL;
        }
        else
            this_tmp_vec[i] = m_vec[i] * scalar;
    }

    delete[] m_vec;
    m_vec = this_tmp_vec;

    return ERR_OK;
}

// @return error code
// @param right - right operand
// @param res - result of operation
// scalar dot product calculation
int IVectorImpl::dotProduct(IVector const* const right, double& res) const
{
    if (!right)
    {
        ILog::report("IVector.dotProduct: right operand of dot product is nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != right->getDim())
    {
        ILog::report("IVector.dotProduct: Dimension mismatch in vector's dot product calculation");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError = ERR_OK;
    const double *p_tmp;
    unsigned int r_dim;
    if ((lastError = right->getCoordsPtr(r_dim, p_tmp)) != ERR_OK)
        return lastError;

    res = 0;

    for (unsigned int i = 0; i < m_dim; i++)
    {
        double tmp = m_vec[i] * tmp;
        if (isnan(res+tmp))
        {
            ILog::report("IVector.dotProduct: double overfulls");
            return ERR_OVERFULL;
        }
        else
            res += tmp;
    }

    return ERR_OK;
}

/*static operations*/

// @return result of addition
// @param left - left operand
// @param right - right operand
// creates a copy of left operand and adds to it the right operand
IVector* IVector::add(IVector const* const left, IVector const* const right)
{
    if (!left || !right)
    {
        ILog::report("IVector.add: operand of addition is nullptr, expected IVector pointer");
        return 0;
    }

    IVector* res = left->clone();
    if(!res)
        return 0;

    if (res->add(right) == ERR_OK)
        return res;
    else
        return 0;
}

// @return result of subtraction
// @param left - left operand
// @param right - right operand
 IVector* IVector::subtract(IVector const* const left, IVector const* const right)
{
    if (!left || !right)
    {
        ILog::report("IVector.subtract: operand of subtraction is nullptr, expected IVector pointer");
        return 0;
    }

    IVector* res = left->clone();
    if(!res)
        return 0;
    if(res->subtract(right) == ERR_OK)
        return res;
    else
        return 0;
}

// @return result of multiplication
// @param left - left operand
// @param scalar - scalar multiplier
IVector* IVector::multiplyByScalar(IVector const* const left, double scalar)
{
    if (!left)
    {
        ILog::report("IVector.multiplyByScalar: Left operand of addition is nullptr, expected IVector pointer");
        return 0;
    }

    IVector* res = left->clone();
    if(!res)
        return 0;

    if(res->multiplyByScalar(scalar) == ERR_OK)
        return res;
    else
        return 0;
}

/*comparators*/

// @return error code
// @param right - right operand
// @param type - norm type
// @param result - variable for result of comparison
// compare vector norms of specified type
int IVectorImpl::gt(IVector const* const right, NormType type, bool& result) const
{

    if (m_dim != right->getDim())
    {
        ILog::report("IVector.gt: Dimension mismatch in gt");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError = ERR_OK;
    double l_norm, r_norm;

    if((lastError = norm(type, l_norm)) != ERR_OK)
        return lastError;

    if((lastError = right->norm(type, r_norm)) != ERR_OK)
        return lastError;

    result = l_norm > r_norm;

    return ERR_OK;
}

// @return error code
// @param right - right operand
// @param type - norm type
// @param result - variable for result of comparison
// compare vector norms of specified type
int IVectorImpl::lt(IVector const* const right, NormType type, bool& result) const
{
    if (m_dim != right->getDim())
    {
        ILog::report("IVector.lt: Dimension mismatch in lt");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError = ERR_OK;
    double l_norm, r_norm;

    if((lastError = norm(type, l_norm)) != ERR_OK)
        return lastError;

    if((lastError = right->norm(type, r_norm)) != ERR_OK)
        return lastError;

    result = l_norm < r_norm;

    return ERR_OK;;

}

// @return error code
// @param right - right operand
// @param type - norm type
// @param result - variable for result of comparison
// @param precision - allowable error
// compare vector norms of specified type
// if the difference between the norms lies in the epsilon neighborhood of zero then norms are equal
int IVectorImpl::eq(IVector const* const right, NormType type, bool& result, double precision) const
{
    if (m_dim != right->getDim())
    {
        ILog::report("IVector.eq: Dimension mismatch in eq");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError = ERR_OK;
    double l_norm, r_norm;

    if((lastError = norm(type, l_norm)) != ERR_OK)
        return lastError;

    if((lastError = right->norm(type, r_norm)) != ERR_OK)
        return lastError;

    if(isnan(r_norm + precision) || isnan(r_norm - precision))
    {
        ILog::report("Ivector.eq: double overfulls");
        return ERR_OVERFULL;
    }

    if(l_norm < r_norm + precision && l_norm > r_norm - precision)
        result = true;
    else
        result = false;

    return ERR_OK;;

}

// @return vector's dimension
unsigned int IVectorImpl::getDim() const
{
    return m_dim;
}

// @return error code
// @param type - norm type
// @param res - variable for result
// calculates norm of the specified type
int IVectorImpl::norm(NormType type, double& res) const
{
    res = 0;
    switch(type)
    {
    case NORM_1:
        for (unsigned int i = 0; i < m_dim; i++)
        {
            if (isnan(res + ABS(m_vec[i])))
            {
                ILog::report("IVector.norm: double overfulls");
                return ERR_OVERFULL;
            }
            else
                res += ABS(m_vec[i]);
        }
        break;

    case NORM_2:
        for (unsigned int i = 0; i < m_dim; i++)
        {
            if(isnan(m_vec[i] * m_vec[i]))
            {
                ILog::report("IVector.norm: double overfulls");
                return ERR_OVERFULL;
            }

            double tmp = m_vec[i] * m_vec[i];
            if (isnan(res+tmp))
            {
                ILog::report("IVector.norm: double overfulls");
                return ERR_OVERFULL;
            }
            else
            res += tmp;
        }
        res = sqrt(res);
        break;

    case NORM_INF:
        for (unsigned int i = 0; i < m_dim; i++)
        {
            if(res < ABS(m_vec[i]))
                res = ABS(m_vec[i]);
        }
        break;

    default:
        ILog::report("IVector.norm: Unknown code of norm");
        return ERR_NORM_NOT_DEFINED;
        break;
    }
    return ERR_OK;
}

// @return error code
// @param index - index of element to set
// @param elem - new value
// set value to index position
int IVectorImpl::setCoord(unsigned int index, double elem)
{
    if (index >= m_dim){
        ILog::report("IVector.setCoord: Index out of range in setCoord");
        return ERR_OUT_OF_RANGE;
    }

    m_vec[index] = elem;
    return ERR_OK;
}

// @return error code
// @param dim - size of array
// @param coords - array of new coords
// set coords values to vector's coords
int IVectorImpl::setAllCoords(unsigned int dim, double* coords)
{
    if (!coords)
    {
        ILog::report("IVector.setAllCoords: setAllCoords got nullptr, expected double pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != dim)
    {
        ILog::report("IVector.setAllCoords: Dimension mismatch in setAllCoords");
        return ERR_DIMENSIONS_MISMATCH;
    }

    for (unsigned int i = 0; i < dim; i++)
    {
        m_vec[i] = coords[i];
    }
    return ERR_OK;
}

// @return error code
// @param other - instance IVector
// change all coords of this vector to other's coords
int IVectorImpl::setAllCoords(IVector const* const other)
{
    if (!other)
    {
        ILog::report("IVector.setAllCoords: setAllCoords got nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != other->getDim())
    {
        ILog::report("IVector.setAllCoords: Dimension mismatch in setAllCoords");
        return ERR_DIMENSIONS_MISMATCH;
    }

    const double *p_tmp;
    unsigned int o_dim;
    int lastError = ERR_OK;
    if ((lastError = other->getCoordsPtr(o_dim, p_tmp)) != ERR_OK)
        return lastError;

    for (unsigned int i = 0; i < m_dim; i++)
    {
        m_vec[i] = p_tmp[i];
    }
    return ERR_OK;
}

// @return error code
// @param index
// @param elem - variable for return value
// gets elem by index
int IVectorImpl::getCoord(unsigned int index, double & elem) const
{
    if (index >= m_dim)
    {
        ILog::report("IVector.getCoord: Index out of range in getCoord method");
        return ERR_OUT_OF_RANGE;
    }

    elem = m_vec[index];
    return ERR_OK;
}

// @return error code
// @param dim - vector's dimension stored here
// @param elem - coord's array
// writes to dim and elem vector's data
int IVectorImpl::getCoordsPtr(unsigned int & dim, double const*& elem) const
{
    dim = m_dim;
    elem = m_vec;
    return ERR_OK;
}

// @return clone this instance of IVector
// create a deep copy
IVector* IVectorImpl::clone() const
{
    double* try_vec = new double[m_dim];
    if(try_vec)
    {
        for(int i = 0; i < m_dim; i++)
        {
            try_vec[i] = m_vec[i];
        }
        return new IVectorImpl(m_dim, try_vec);
    }
    else
    {
        ILog::report("IVector.clone: not enough memory");
        return 0;
    }
}

// default D-TOR
IVectorImpl::~IVectorImpl()
{
    delete[] m_vec;
}
