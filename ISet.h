#ifndef ISET_H
#define ISET_H

#include "IVector.h"
#include "SHARED_EXPORT.h"

class SHARED_EXPORT ISet
{
public:

    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    virtual int getId() const = 0;

    /*factories*/
    static ISet* createSet(unsigned int R_dim);

    virtual int put(IVector const* const item) = 0;
    virtual int get(unsigned int index, IVector*& pItem) const = 0;
    virtual int remove(unsigned int index) = 0;
    virtual int contains(IVector const* const pItem, bool & rc) const = 0;
    virtual unsigned int getSize() const = 0;
    virtual int clear() = 0;

    class IIterator
    {
    public:
        virtual int next() = 0;
        virtual int prev() = 0;
        virtual bool isEnd() const = 0;
        virtual bool isBegin() const = 0;

    protected:
        IIterator(ISet const* const set, int pos);

        /*dtor*/
        virtual ~IIterator(){};

    private:
        /*non default copyable*/
        IIterator(const IIterator& other) = delete;
        void operator=(const IIterator& other) = delete;
    };

    virtual IIterator* end() = 0;
    virtual IIterator* begin() = 0;

    virtual int deleteIterator(IIterator * pIter) = 0;
    virtual int getByIterator(IIterator const* pIter, IVector*& pItem) const = 0;

    /*dtor*/
    virtual ~ISet(){};

protected:
    ISet() = default;

private:
    /*non default copyable*/
    ISet(const ISet& other) = delete;
    void operator=(const ISet& other) = delete;
};

#endif // ISET_H