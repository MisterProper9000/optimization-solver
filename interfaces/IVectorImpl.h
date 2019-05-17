#ifndef IVECTORIMPL_H
#define IVECTORIMPL_H

#include "IVector.h"
#define try(func) if(func != 0) ILogImpl.report("sas")


namespace {
    class IVectorImpl: public virtual IVector
    {
        int getId() const override;
        /*non default copyable*/
        IVectorImpl(const IVectorImpl& other) = delete;
        void operator=( const IVectorImpl& other) = delete;

        /*operations*/
        virtual int add(IVector const* const right);
        virtual int subtract(IVector const* const right);
        virtual int multiplyByScalar(double scalar);
        virtual int dotProduct(IVector const* const right, double& res) const;
        virtual int crossProduct(IVector const* const right);

        /*comparators*/
        virtual int gt(IVector const* const right, NormType type, bool& result) const;
        virtual int lt(IVector const* const right, NormType type, bool& result) const;
        virtual int eq(IVector const* const right, NormType type, bool& result) const;

        /*utils*/
        virtual unsigned int getDim() const;

        virtual int norm(NormType type, double& res) const;
        virtual int setCoord(unsigned int index, double elem);
        virtual int setAllCoords(unsigned int dim, double* coords);
        virtual int setAllCoords(IVector const* const other);
        virtual int getCoordPtr(unsigned int index, double*& elem) const;
        virtual IVector* clone() const;

        /*dtor*/
        virtual ~IVectorImpl() = default;

    protected:
        IVectorImpl() = default;

    };

}
#endif // IVECTORIMPL_H
