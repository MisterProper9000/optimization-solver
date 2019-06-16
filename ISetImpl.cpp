#include "ISet.h"
#include "ILog.h"
#include <QVector>

namespace {
    class ISetImpl: public virtual ISet
    {
    public:
        virtual int getId() const;

        /*factories*/
        static ISet* createSet(unsigned int R_dim);

        virtual int put(IVector const* const item);
        virtual int get(unsigned int index, IVector*& pItem) const;
        virtual int remove(unsigned int index);
        virtual int contains(IVector const* const pItem, bool & rc) const;
        virtual unsigned int getSize() const;
        virtual int clear();

        virtual ISet::IIterator* end();
        virtual ISet::IIterator* begin();

        virtual int deleteIterator(IIterator * pIter);
        virtual int getByIterator(IIterator const* pIter, IVector*& pItem) const;

        class IIteratorImpl: public ISet::IIterator
        {
        public:
            virtual int next();
            virtual int prev();
            virtual bool isEnd() const;
            virtual bool isBegin() const;
            const ISet* getSet() const {return m_set;}
            unsigned int getPos() const {return m_pos;}

            IIteratorImpl(ISet const* const set, int pos);
            virtual ~IIteratorImpl();
        private:
            /*non default copyable*/
            const ISet* m_set;
            unsigned int m_pos;
            IIteratorImpl(const IIteratorImpl& other) = delete;
            void operator=(const IIteratorImpl& other) = delete;
        };

        /*dtor*/
        ~ISetImpl();

    protected:
        ISetImpl(unsigned int R_dim);
    private:
        QVector <IIteratorImpl*> iteratorList;
        QVector <const IVector*> m_data;
        unsigned int m_dim;
        /*non default copyable*/
        ISetImpl(const ISetImpl& other) = delete;
        void operator=(const ISetImpl& other) = delete;
    };

}
//------------------------------------------------------------------------------------------------
/**
    Returns id of interface
 */
int ISetImpl::getId() const
{
    return DIMENSION_INTERFACE_IMPL;
}
//------------------------------------------------------------------------------------------------
/**
    Creates empty set of base class by dimension
    @param R_dim dimension
 */
ISet* ISet::createSet(unsigned int R_dim)
{
    return ISetImpl::createSet(R_dim);
}
//------------------------------------------------------------------------------------------------
/**
    Creates empty set of derived class by dimension
    @param R_dim dimension
 */
ISet* ISetImpl::createSet(unsigned int R_dim)
{
    if (R_dim == 0) {
        ILog::report("Error: can't create set with zero dimension");
        return 0;
    }
    return new ISetImpl(R_dim);
}
//------------------------------------------------------------------------------------------------
/**
    CTOR of derived Set
    @param R_dim dimension
 */
ISetImpl::ISetImpl(unsigned int R_dim): m_dim(R_dim) {
    //ILog::report("CTOR set");
}
//------------------------------------------------------------------------------------------------
/**
    DTOR of derived Set
 */
ISetImpl::~ISetImpl() {
    //ILog::report("DTOR set");
    clear();
}
//------------------------------------------------------------------------------------------------
/**
    Puts vector in Set
    @param item Vector to put
 */
int ISetImpl::put(IVector const* const item)
{
    if (!item)
    {
        ILog::report("Error: push invalid vector");
        return ERR_WRONG_ARG;
    }
    if (item->getDim() != m_dim)
    {
        ILog::report("Error: push vector with wrong dimension");
        return ERR_DIMENSIONS_MISMATCH;
    }
    m_data.push_back(item);
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Gets an element from Set by index
    @param index index of element to get
    @param pItem returned element
 */
int ISetImpl::get(unsigned int index, IVector*& pItem) const
{
    if (getSize() == 0)
    {
        ILog::report("Error: the Set is empty, can't get any item");
        return ERR_ANY_OTHER;
    }
    if (!pItem)
    {
        ILog::report("Error: invalid vector to get data");
        return ERR_WRONG_ARG;
    }
    if (index >= (unsigned int)m_data.size()) {
        ILog::report("Error: get element of vector out of range");
        return ERR_OUT_OF_RANGE;
    }
    if (pItem->getDim() != m_dim)
    {
        ILog::report("Error: get vector with wrong dimension");
        return ERR_DIMENSIONS_MISMATCH;
    }
    pItem = const_cast<IVector*>(m_data[index]);
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Removes an element from Set by index
    @param index index of element to remove
 */
int ISetImpl::remove(unsigned int index)
{
    if (index >= (unsigned int)m_data.size()) {
        ILog::report("Error: remove element of vector out of range");
        return ERR_OUT_OF_RANGE;
    }
    m_data.remove(index);
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Check if Ivector is in Set
    @param pItem element to check
    @param rc result (T/F)
 */
int ISetImpl::contains(IVector const* const pItem, bool &rc) const
{
    if (getSize() == 0)
    {
        rc = false;
        ILog::report("Error: the Set is empty");
        return ERR_ANY_OTHER;
    }
    if (pItem->getDim() != m_dim)
    {
        rc = false;
        ILog::report("Error: checking vector with wrong dimension");
        return ERR_DIMENSIONS_MISMATCH;
    }
    rc = m_data.contains(pItem);
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Gets number of points in Set
 */
unsigned int ISetImpl::getSize() const
{
    return m_data.size();
}
//------------------------------------------------------------------------------------------------
/**
    Clear Set
 */
int ISetImpl::clear()
{
   m_data.clear();
   iteratorList.clear();
   return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    CTOR of Base Iterator
 */
ISet::IIterator::IIterator(ISet const* const set, int pos) {}
//------------------------------------------------------------------------------------------------
/**
    CTOR of Derived Iterator
 */
ISetImpl::IIteratorImpl::IIteratorImpl(ISet const* const set, int pos) :
    ISet::IIterator::IIterator(set, pos)
{
    m_set = set;
    m_pos = pos;
}
//------------------------------------------------------------------------------------------------
/**
    DTOR of Derived Iterator
 */
ISetImpl::IIteratorImpl::~IIteratorImpl()
{
   this->m_set = 0;
   this->m_pos = 0;
}
//------------------------------------------------------------------------------------------------
/**
    Returns end Iterator
 */
ISetImpl::IIterator* ISetImpl::end()
{
    if (getSize() == 0)
    {
        ILog::report("Error: can't iterate, the Set is empty");
        return 0;
    }
    ISetImpl::IIteratorImpl* it = new ISetImpl::IIteratorImpl(this, getSize() - 1);
    this->iteratorList.push_back(it);
    return it;
}
//------------------------------------------------------------------------------------------------
/**
    Returns begin Iterator
 */
ISetImpl::IIterator* ISetImpl::begin()
{
    if (getSize() == 0)
    {
        ILog::report("Error: can't iterate, the Set is empty");
        return 0;
    }
    ISetImpl::IIteratorImpl* it = new ISetImpl::IIteratorImpl(this, 0);
    this->iteratorList.push_back(it);
    return it;
}
//------------------------------------------------------------------------------------------------
/**
    Deletes iterator from Iterator storage
    @param pIter Iterator to delete
 */
int ISetImpl::deleteIterator(IIterator * pIter)
{
    if (!pIter)
    {
        ILog::report("Error: invalid argument in function deleteIterator()");
        return ERR_WRONG_ARG;
    }

    for (int i = 0; i < iteratorList.size(); i++)
    {
        if (pIter == iteratorList[i])
        {
            delete iteratorList[i];
            iteratorList.remove(i);
        }
    }
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Returns point of Set by Iterator
    @param pIter Iterator to get point
    @param pItem returning value (point)
 */
int ISetImpl::getByIterator(IIterator const* pIter, IVector*& pItem) const
{
    if (getSize() == 0)
    {
        ILog::report("Error: the Set is empty");
        return ERR_ANY_OTHER;
    }
    if (!pIter || !pItem) {
        ILog::report("Error: invalid argument in function getByIterator()");
        return ERR_WRONG_ARG;
    }
    if (pItem->getDim() != m_dim)
    {
        ILog::report("Error: get vector with wrong dimension");
        return ERR_DIMENSIONS_MISMATCH;
    }
    unsigned int idx = static_cast<const IIteratorImpl*>(pIter)->getPos();
    if (idx < 0 || idx >= getSize())
    {
        ILog::report("Error: wrong argument, iterator out of range");
        return ERR_OUT_OF_RANGE;
    }
    pItem = const_cast<IVector*>(m_data[idx]);
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Gets next Iterator
 */
int ISetImpl::IIteratorImpl::next()
{
    if (this->m_pos == m_set->getSize() - 1) {
        ILog::report("Error: iterator can't move out of range");
        return ERR_OUT_OF_RANGE;
    }
    this->m_pos += 1;
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Gets previous Iterator
 */
int ISetImpl::IIteratorImpl::prev()
{
    if (this->m_pos == 0) {
        ILog::report("Error: iterator can't move out of range");
        return ERR_OUT_OF_RANGE;
    }
    this->m_pos -= 1;
    return ERR_OK;
}
//------------------------------------------------------------------------------------------------
/**
    Checks if Iterator in the end
 */
bool ISetImpl::IIteratorImpl::isEnd() const
{
    return (this->m_pos == m_set->getSize() - 1);
}
//------------------------------------------------------------------------------------------------
/**
    Checks if Iterator is begin Iterator
 */
bool ISetImpl::IIteratorImpl::isBegin() const
{
    return (this->m_pos == 0);
}
//------------------------------------------------------------------------------------------------



