#ifndef IVECTOR_H
#define IVECTOR_H

class IVector
{
public:
    enum ErrorEnum {
        ERR_OK,
        ERR_WRONG_ARG,
        ERR_OUT_OF_MEMORY,
        ERR_DIMENSIONS_MISMATCH,
    };

    enum NormType
    {
        NORM_1,
        NORM_2,
        NORM_INF
    };

    /*non default copyable*/
    IVector(const IVector& other) = delete;
    void operator=( const IVector& other) = delete;

    /*factories*/
    static IVector* createVector(unsigned int size);

    /*operations*/
    virtual int add(IVector const* const right) = 0;
    virtual int subtract(IVector const* const right) = 0;
    virtual int multiplyByScalar(double scalar) = 0;
    virtual int dotProduct(IVector const* const right) = 0;
    virtual int crossProduct(IVector const* const right) = 0;
    virtual int norm(NormType type, double& res) const = 0;

    /*comparators*/
    virtual int gt(IVector const* const right, NormType type, bool& result) const = 0;
    virtual int lt(IVector const* const right, NormType type, bool& result) const = 0;
    virtual int eq(IVector const* const right, NormType type, bool& result) const = 0;

    /*utils*/
    virtual unsigned int getDim() const = 0;
    virtual int setCoord(unsigned int index, double elem) = 0;
    virtual int setAllCoords(unsigned int dim, double* coords) = 0;
    virtual int setAllCoords(IVector const* const other) = 0;
    virtual int getCoordPtr(unsigned int index, double*& elem) const = 0;
    virtual IVector* clone() const = 0;

    /*dtor*/
    virtual ~IVector() = default;

protected:
    IVector() = default;

};

#endif // IVECTOR_H
