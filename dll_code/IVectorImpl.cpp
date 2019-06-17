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
        IVectorImpl(unsigned int size, double const* vals);
        QVector< double > m_vec;
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
        ILog::report("Error: Vector init with nullptr, expected double pointer");
        return 0;
    }
    if (size == 0)
    {
        ILog::report("Error: Vector can't be zero-dimensional");
        return 0;
    }

    return IVectorImpl::createVector(size, vals);
}

IVector* IVectorImpl::createVector(unsigned int size, const double *vals)
{
    return new IVectorImpl(size, vals);
}

// @param size - legth of data array
// @param vals - data array
// creates VectorImpl
IVectorImpl::IVectorImpl(unsigned int size, double const* vals): m_dim(size)
{
    //std::cout<<"ctor"<<std::endl;
    for (unsigned int i = 0; i < m_dim; i++)
    {
        m_vec.push_back(vals[i]);
    }
}

/*operations*/

// @return error code
// @param right - right operand
// coordinatewise addition: this + right
int IVectorImpl::add(IVector const* const right)
{
    if (!right)
    {
        ILog::report("Error: right operand of addition is nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != right->getDim())
    {
        ILog::report("Error: Dimension mismatch in vector's addition");
        return ERR_DIMENSIONS_MISMATCH;
    }
    int lastError = ERR_OK;
    const double *p_tmp;
    unsigned int r_dim;
    if ((lastError = right->getCoordsPtr(r_dim, p_tmp)) != ERR_OK)
        return lastError;

    for (unsigned int i = 0; i < m_dim; i++)
    {
        if (p_tmp[i] > 0 && m_vec[i] > 0 && (DBL_MAX - p_tmp[i] < m_vec[i] || DBL_MAX - m_vec[i] < p_tmp[i]))
        {
            ILog::report("WARNING: IVector.add overfulls, result saturated to DBL_MAX");
            m_vec[i] = DBL_MAX;
            lastError = ERR_OVERFULL;
        }
        else if (p_tmp[i] < 0 && m_vec[i] < 0 && (-DBL_MAX - p_tmp[i] > m_vec[i] || -DBL_MAX - m_vec[i] > p_tmp[i]))
        {
            ILog::report("WARNING: IVector.add overfulls, result saturated to -DBL_MAX");
            m_vec[i] = -DBL_MAX;
            lastError = ERR_OVERFULL;
        }
        else
            m_vec[i] += p_tmp[i];
    }
    return lastError;
}

// @return error code
// @param right - right operand
// coordinatewise subtraction: this - right
int IVectorImpl::subtract(IVector const* const right)
{
    if (!right)
    {
        ILog::report("Error: right operand of subtraction is nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != right->getDim())
    {
        ILog::report("Error: Dimension mismatch in vector's subtraction");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError;
    const double *p_tmp;
    unsigned int r_dim;
    if ((lastError = right->getCoordsPtr(r_dim, p_tmp)) != ERR_OK)
        return lastError;

    for (unsigned int i = 0; i < m_dim; i++)
    {
        if (p_tmp[i] < 0 && m_vec[i] > 0 && (DBL_MAX + p_tmp[i] < m_vec[i] || DBL_MAX - m_vec[i] < -p_tmp[i]))
        {
            ILog::report("WARNING: IVector.subtract overfulls, result saturated to DBL_MAX");
            m_vec[i] = DBL_MAX;
            lastError = ERR_OVERFULL;
        }
        else if (p_tmp[i] > 0 && m_vec[i] < 0 && (-DBL_MAX + p_tmp[i] > m_vec[i] || -DBL_MAX - m_vec[i] > -p_tmp[i]))
        {
            ILog::report("WARNING: IVector.subtract overfulls, result saturated to -DBL_MAX");
            m_vec[i] = -DBL_MAX;
            lastError = ERR_OVERFULL;
        }
        else
            m_vec[i] -= p_tmp[i];
    }
    return lastError;
}

// @return error code
// @param scalar - scalar multiplier
// coordinatewise multiplication: this * scalar
int IVectorImpl::multiplyByScalar(double scalar)
{
    int lastError = ERR_OK;
    for (unsigned int i = 0; i < m_dim; i++)
    {
        if(DBL_MAX/ABS(m_vec[i]) < ABS(scalar) || DBL_MAX/ABS(scalar) < ABS(m_vec[i]))
        {
            ILog::report("WARNING: IVector.multiplyByScalar overfulls, result saturated to DBL_MAX");
            double tmp1 = SIGN(scalar);
            double tmp2 = SIGN(m_vec[i]);
            m_vec[i] = (tmp1*tmp2>0)?DBL_MAX:(tmp1*tmp2<0)?-DBL_MAX:0;
            lastError = ERR_OVERFULL;
        }
        else
            m_vec[i] *= scalar;
    }
    return lastError;
}

// @return error code
// @param right - right operand
// @param res - result of operation
// scalar dot product calculation
int IVectorImpl::dotProduct(IVector const* const right, double& res) const
{
    if (!right)
    {
        ILog::report("Error: right operand of dot product is nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != right->getDim())
    {
        ILog::report("Error: Dimension mismatch in vector's dot product calculation");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError;
    const double *p_tmp;
    unsigned int r_dim;
    if ((lastError = right->getCoordsPtr(r_dim, p_tmp)) != ERR_OK)
        return lastError;

    res = 0;

    for (unsigned int i = 0; i < m_dim; i++)
    {
        double tmp = m_vec[i] * tmp;
        if (tmp > 0 && res > 0 && (DBL_MAX - tmp < res || DBL_MAX - res < tmp))
        {
            ILog::report("Error: IVector.dotProduct overfulls, result saturated to DBL_MAX");
            res = DBL_MAX;
            return ERR_OVERFULL;
        }
        else if (tmp < 0 && res < 0 && (-DBL_MAX - tmp > res || -DBL_MAX - res > tmp))
        {
            ILog::report("Error: IVector.dotProduct overfulls, result saturated to -DBL_MAX");
            res = -DBL_MAX;
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
        ILog::report("Error: operand of addition is nullptr, expected IVector pointer");
        return 0;
    }

    IVector* res = left->clone();
    res->add(right);
    return res;
}

// @return result of subtraction
// @param left - left operand
// @param right - right operand
 IVector* IVector::subtract(IVector const* const left, IVector const* const right)
{
    if (!left || !right)
    {
        ILog::report("Error: operand of subtraction is nullptr, expected IVector pointer");
        return 0;
    }

    IVector* res = left->clone();
    res->subtract(right);
    return res;
}

// @return result of multiplication
// @param left - left operand
// @param scalar - scalar multiplier
IVector* IVector::multiplyByScalar(IVector const* const left, double scalar)
{
    if (!left)
    {
        ILog::report("Error: Left operand of addition is nullptr, expected IVector pointer");
        return 0;
    }

    IVector* res = left->clone();
    res->multiplyByScalar(scalar);
    return res;
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
        ILog::report("Error: Dimension mismatch in gt");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError;
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
        ILog::report("Error: Dimension mismatch in lt");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError;
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
        ILog::report("Error: Dimension mismatch in eq");
        return ERR_DIMENSIONS_MISMATCH;
    }

    int lastError;
    double l_norm, r_norm;

    if((lastError = norm(type, l_norm)) != ERR_OK)
        return lastError;

    if((lastError = right->norm(type, r_norm)) != ERR_OK)
        return lastError;

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
            if (ABS(m_vec[i]) > 0 && res > 0 && (DBL_MAX - ABS(m_vec[i]) < res || DBL_MAX - res < ABS(m_vec[i])))
            {
                ILog::report("Error: IVector.norm overfulls, result saturated to DBL_MAX");
                res = DBL_MAX;
                return ERR_OVERFULL;
            }
            else if (ABS(m_vec[i]) < 0 && res < 0 && (-DBL_MAX - ABS(m_vec[i]) > res || -DBL_MAX - res > ABS(m_vec[i])))
            {
                ILog::report("Error: IVector.norm overfulls, result saturated to -DBL_MAX");
                res = -DBL_MAX;
                return ERR_OVERFULL;
            }
            else
                res += ABS(m_vec[i]);
        }
        break;

    case NORM_2:
        for (unsigned int i = 0; i < m_dim; i++)
        {
            double tmp = m_vec[i] * m_vec[i];;
            if (tmp > 0 && res > 0 && (DBL_MAX - tmp < res || DBL_MAX - res < tmp))
            {
                ILog::report("Error: IVector.norm overfulls, result saturated to DBL_MAX");
                res = DBL_MAX;
                return ERR_OVERFULL;
            }
            else if (tmp < 0 && res < 0 && (-DBL_MAX - tmp > res || -DBL_MAX - res > tmp))
            {
                ILog::report("Error: IVector.norm overfulls, result saturated to -DBL_MAX");
                res = -DBL_MAX;
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
        ILog::report("Error: Unknown code of norm");
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
        ILog::report("Error: Index out of range in setCoord");
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
        ILog::report("Error: setAllCoords got nullptr, expected double pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != dim)
    {
        ILog::report("Error: Dimension mismatch in setAllCoords");
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
        ILog::report("Error: setAllCoords got nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }

    if (m_dim != other->getDim())
    {
        ILog::report("Error: Dimension mismatch in setAllCoords");
        return ERR_DIMENSIONS_MISMATCH;
    }

    const double *p_tmp;
    unsigned int o_dim;
    int lastError;
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
        ILog::report("Error: Index out of range in getCoord method");
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
    elem = m_vec.data();
    return ERR_OK;
}

// @return clone this instance of IVector
// create a deep copy
IVector* IVectorImpl::clone() const
{
    return new IVectorImpl(m_dim, m_vec.data());
}

// default D-TOR
IVectorImpl::~IVectorImpl(){/*std::cout<<"dtor"<<std::endl;*/}
