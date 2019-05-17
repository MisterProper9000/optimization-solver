#ifndef ISET_H
#define ISET_H

#include "error.h"
#include "SHARED_EXPORT.h"

class IVector;
class SHARED_EXPORT ISet
{
public:
    /*factories*/
    static ISet* createSet(unsigned int R_dim);

    virtual int put(IVector const* const item) = 0;
    virtual int get(unsigned int index, IVector*& pItem) const = 0;
    virtual int remove(unsigned int index) = 0;
    virtual int contains(IVector const* const pItem) const = 0;
    virtual unsigned int getSize() const = 0;
    virtual int clear() = 0;

    /*dtor*/
    virtual ~ISet() = default;

    virtual iterator* end() = 0;
    virtual iterator* begin() = 0;

    class iterator
    {
    public:
        virtual int doStep();

    protected:
        iterator(int step);

        virtual int next() = 0;
        virtual int prev() = 0;

        /*dtor*/
        virtual ~iterator() = default;

    private:
        /*non default copyable*/
        iterator(const iterator& other) = delete;
        void operator=( const iterator& other) = delete;
    };

protected:
    Set() = default;

private:
    /*non default copyable*/
    ISet(const ISet& other) = delete;
    void operator=( const ISet& other) = delete;
};

#endif // ISET_H
