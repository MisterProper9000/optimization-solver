#ifndef ICOMPACT_H
#define ICOMPACT_H

#include "util.h"
#include <QVector>
#include <QPair>
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
    static ICompact* createCompact(IVector const* const begin, IVector const* const end);

    /*operations*/
    virtual int Intersection(ICompact const& c) = 0;
    virtual int Union(ICompact const& c) = 0;
    virtual int Difference(ICompact const& c) = 0;
    virtual int SymDifference(ICompact const& c) = 0;
    virtual int MakeConvex() = 0;

    /*static operations*/
    static ICompact* Intersection(ICompact const* const left, ICompact const* const right);
    static ICompact* Union(ICompact const* const left, ICompact const* const right);
    static ICompact* Difference(ICompact const* const left, ICompact const* const right);
    static ICompact* SymDifference(ICompact const* const left, ICompact const* const right);
    static ICompact* MakeConvex(ICompact const* const left, ICompact const* const right);

    /* returns a step with which you can iterate over all domains of compact*/
    virtual IVector* getStep() const = 0;

    virtual iterator* end(IVector const* const step = 0) = 0;
    virtual iterator* begin(IVector const* const step = 0) = 0;

    virtual int isContains(IVector const* const vec, bool& result) const = 0;
    virtual int isSubSet(ICompact const* const other) = 0;
    virtual int isSimplyConn(bool& result) = 0;
    virtual int isIntersects(ICompact const* const other, bool& result) = 0;

    virtual ICompact* clone() const = 0;

    /*dtor*/
    virtual ~ICompact() = default;

    class iterator
    {
    public:
        //adds step to current value in iterator
        virtual int doStep() = 0;

        //change step
        virtual int setDirection(IVector const* const dir) = 0;


    protected:
        iterator(IVector const* const step);

        /*dtor*/
        virtual ~iterator() = default;

    private:
        /*non default copyable*/
        iterator(const iterator& other) = delete;
        void operator=( const iterator& other) = delete;
    };

private:
    /*non default copyable*/
    ICompact(const ICompact& other) = delete;
    void operator=( const ICompact& other) = delete;
};

#endif // ICOMPACT_H
