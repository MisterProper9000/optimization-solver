#ifndef ICOMPACT_H
#define ICOMPACT_H

#include <QVector>
#include <QPair>
#include <QSharedPointer>
#include "IVector.h"

class ICompact
{
public:
    enum ErrorEnum {
        ERR_OK,
        ERR_WRONG_ARG,
        ERR_OUT_OF_MEMORY,
        ERR_DIMENSIONS_MISMATCH,
        ERR_INVALID_STEP,
    };

    /*non default copyable*/
    ICompact(const ICompact& other) = delete;
    void operator=( const ICompact& other) = delete;

    /*factories*/
    static ICompact* createCompact(IVector const* const begin, IVector const* const end);
    static ICompact* createCompact(int domainsCount, IVector const* const begin, IVector const* const end,...);

    /*operations*/
    virtual int Intersection(ICompact const& c) = 0;
    virtual int Union(ICompact const& c) = 0;
    virtual int Difference(ICompact const& c) = 0;
    virtual int SymDifference(ICompact const& c) = 0;
    virtual int MakeConvex() = 0;

    /*utils*/
    //virtual QPair<QSharedPointer<IVector>, QSharedPointer<IVector> > getBoundaries() const = 0;
    //virtual QPair<double, double> getBoundaries(unsigned int dim) = 0;
    virtual IVector* getMaxStep() const = 0;
    virtual IVector* getDomainStep(unsigned int domainIdx) const = 0;

    virtual iterator* end(unsigned int idx) = 0;
    virtual iterator* begin(unsigned int idx) = 0;

    virtual int isContains(IVector const* const vec, bool& result) const = 0;
    virtual int isSubSet(ICompact const* const other) = 0;
    virtual int isSimplyConn(bool& result) = 0;
    virtual int isIntersects(ICompact const* const other, bool& result) = 0;

    virtual int domainCount(unsigned int& result) = 0;
    //virtual ICompact* getDomain(unsigned int domainIdx) = 0;

    virtual ICompact* clone() const = 0;
    virtual ICompact* cloneDomain(unsigned int domainIdx) const = 0;

    /*dtor*/
    virtual ~ICompact() = default;

    class iterator
    {
    public:
        /*non default copyable*/
        iterator(const iterator& other) = delete;
        void operator=( const iterator& other) = delete;

        virtual iterator* begin() = 0;
        virtual iterator* end() = 0;

        virtual int setDirection(IVector const* const dir) = 0;


    protected:
        iterator() = default;
        //virtual int isDirectionValid() = 0;
        /*dtor*/
        virtual ~iterator() = default;
    };

protected:
/*
    ICompact() = default;


    class ISimplyConnectedCompact
    {
    public:
        //non default copyable
        ISimplyConnectedCompact(const ISimplyConnectedCompact& other) = delete;
        void operator=( const ISimplyConnectedCompact& other) = delete;

        //ctor
        ISimplyConnectedCompact(IVector const* const begin, IVector const* const end);

        //operations
        virtual int Intersection(ISimplyConnectedCompact const* const right) = 0;
        virtual int Union(ISimplyConnectedCompact const* const right) = 0;
        virtual int Difference(ISimplyConnectedCompact const* const right) = 0;
        virtual int SymDifference(ISimplyConnectedCompact const* const right) = 0;

        //utils
        virtual QPair<QSharedPointer<IVector>, QSharedPointer<IVector> > getBoundaries() = 0;
        virtual QPair<double, double> getBoundaries(unsigned int dim) = 0;
        virtual IVector* getStep() = 0;

        virtual int isContains(IVector const* const vec, bool& result) const = 0;
        virtual int isSubSet(ISimplyConnectedCompact const* const other) = 0;
        virtual int isSimplyConn(bool& result) = 0;
        virtual int isIntersects(ISimplyConnectedCompact const* const other, bool& result) = 0;

        virtual ISimplyConnectedCompact* clone() const = 0;

        //dtor
        virtual ~ISimplyConnectedCompact() = default;
    };*/
};

#endif // ICOMPACT_H
