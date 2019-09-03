#ifndef IVECTOR_H
#define IVECTOR_H

#include "error.h"
#include "SHARED_EXPORT.h"

class SHARED_EXPORT IVector
{
public:
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    enum NormType
    {
        NORM_1,
        NORM_2,
        NORM_INF,
        DIMENSION_NORM
    };

    virtual int getId() const = 0;

    /*factories*/
    static IVector* createVector(unsigned int size, double const* vals);

    /*operations*/
    virtual int add(IVector const* const right) = 0;
    virtual int subtract(IVector const* const right) = 0;
    virtual int multiplyByScalar(double scalar) = 0;
    virtual int dotProduct(IVector const* const right, double& res) const = 0;
    virtual int crossProduct(IVector const* const right)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return ERR_NOT_IMPLEMENTED;
    }

    /*static operations*/
    static IVector* add(IVector const* const left, IVector const* const right);
    static IVector* subtract(IVector const* const left, IVector const* const right);
    static IVector* multiplyByScalar(IVector const* const left, double scalar);
    static IVector* crossProduct(IVector const* const left, IVector const* const right)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return static_cast<IVector*>(0);
    }

    /*comparators*/
    virtual int gt(IVector const* const right, NormType type, bool& result) const = 0;
    virtual int lt(IVector const* const right, NormType type, bool& result) const = 0;
    virtual int eq(IVector const* const right, NormType type, bool& result, double precision) const = 0;

    /*utils*/
    virtual unsigned int getDim() const = 0;
    virtual int norm(NormType type, double& res) const = 0;
    virtual int setCoord(unsigned int index, double elem) = 0;
    virtual int getCoord(unsigned int index, double & elem) const = 0;
    virtual int setAllCoords(unsigned int dim, double* coords) = 0;
    virtual int getCoordsPtr(unsigned int & dim, double const*& elem) const = 0;
    virtual IVector* clone() const = 0;

    /*dtor*/
    virtual ~IVector(){};

protected:
    IVector() = default;

private:
    /*non default copyable*/
    IVector(const IVector& other) = delete;
    void operator=(const IVector& other) = delete;
};

#endif // IVECTOR_H
