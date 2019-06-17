#ifndef ICOMPACT_H
#define ICOMPACT_H

#include "IVector.h"
#include "SHARED_EXPORT.h"

class SHARED_EXPORT ICompact
{
public:
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    virtual int getId() const = 0;

    /*factories*/
    static ICompact* createCompact(IVector const* const begin, IVector const* const end, IVector const* const step = 0);

    /*operations*/
    virtual int Intersection(ICompact const& c)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return ERR_NOT_IMPLEMENTED;
    }
    virtual int Union(ICompact const& c)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return ERR_NOT_IMPLEMENTED;
    }
    virtual int Difference(ICompact const& c)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return ERR_NOT_IMPLEMENTED;
    }
    virtual int SymDifference(ICompact const& c)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return ERR_NOT_IMPLEMENTED;
    }
    virtual int MakeConvex() { return ERR_OK; }

    /*static operations*/
    static ICompact* Intersection(ICompact const* const left, ICompact const* const right)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return static_cast<ICompact*>(0);
    }
    static ICompact* Union(ICompact const* const left, ICompact const* const right)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return static_cast<ICompact*>(0);
    }
    static ICompact* Difference(ICompact const* const left, ICompact const* const right)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return static_cast<ICompact*>(0);
    }
    static ICompact* SymDifference(ICompact const* const left, ICompact const* const right)
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return static_cast<ICompact*>(0);
    }
    static ICompact* MakeConvex(ICompact const* const src)
    {
        if (!src) return static_cast<ICompact*>(0);
        else return src->clone();
    }

    virtual int deleteIterator(IIterator * pIter) = 0;
    virtual int getByIterator(IIterator const* pIter, IVector*& pItem) const = 0;

    virtual IIterator* end(IVector const* const step = 0) = 0;
    virtual IIterator* begin(IVector const* const step = 0) = 0;

    virtual int isContains(IVector const* const vec, bool& result) const = 0;
    virtual int isSubSet(ICompact const* const other) const = 0;
    virtual int isSimplyConn(bool& result) const
    {
        result = true;
        return ERR_OK;
    }
    virtual int isIntersects(ICompact const* const other, bool& result) const
    {
        qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
        return ERR_NOT_IMPLEMENTED;
    }
    virtual int getNearestNeighbor(IVector const* vec, IVector *& nn) const = 0;

    virtual ICompact* clone() const = 0;

    /*dtor*/
    virtual ~ICompact(){};

    class IIterator
    {
    public:
        //adds step to current value in iterator
        virtual int doStep() = 0;

        //change step
        virtual int setStep(IVector const* const step) = 0;


    protected:
        IIterator(ICompact const* const compact, int pos, IVector const* const step);

        /*dtor*/
        virtual ~IIterator(){};

    private:
        /*non default copyable*/
        IIterator(const IIterator& other) = delete;
        void operator=(const IIterator& other) = delete;
    };

protected:
    ICompact() = default;

private:
    /*non default copyable*/
    ICompact(const ICompact& other) = delete;
    void operator=(const ICompact& other) = delete;
};

#endif // ICOMPACT_H