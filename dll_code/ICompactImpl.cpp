#include "ICompact.h"
#include "ILog.h"
#include <QVector>
#include <cfloat>
#include <limits.h>
#include <math.h>

#define ABS(x) ((x) > 0 ? (x) : -(x))
#define SIGN(x) (x >= 0) ? ((x>0)?(1):(0)):(-1)

namespace{
    class ICompactImpl: public virtual ICompact
    {
    public:
        virtual int getId() const;

        /*factories*/
        static ICompact* createCompact(IVector const* const begin, IVector const* const end, IVector const* const step = 0);

        virtual int deleteIterator(IIterator * pIter);
        virtual int getByIterator(IIterator const* pIter, IVector*& pItem) const;

        virtual IIterator* end(IVector const* const step = 0);
        virtual IIterator* begin(IVector const* const step = 0);

        virtual int isContains(IVector const* const vec, bool& result) const;
        virtual int isSubSet(ICompact const* const other) const;


        virtual int getNearestNeighbor(IVector const* vec, IVector *& nn) const;

        virtual ICompact* clone() const;

        /*dtor*/
        virtual ~ICompactImpl();

        void vecToIdx(IVector const* const vec, unsigned int& res) const;
        void idxToVec(unsigned int idx, IVector*& res) const;

        class IIteratorImpl: public virtual ICompact::IIterator
        {
        public:
            //adds step to current value in iterator
            virtual int doStep();

            //change step
            virtual int setStep(IVector const* const step);

            const ICompact* getCompact() const {return m_com;}
            unsigned int getPos() const {return m_pos;}

            /*dtor*/
            virtual ~IIteratorImpl();
            IIteratorImpl(ICompact const* const compact, int pos, IVector const* const step);
        protected:




        private:
            const ICompact* m_com;
            unsigned int m_pos;
            IVector* m_step;

            /*non default copyable*/
            IIteratorImpl(const IIteratorImpl& other) = delete;
            void operator=(const IIteratorImpl& other) = delete;
        };

    protected:
        ICompactImpl() = default;
        ICompactImpl(IVector* begin, IVector* end, IVector* step, unsigned int card);

    private:

        /*non default copyable*/
        ICompactImpl(const ICompactImpl& other) = delete;
        void operator=(const ICompactImpl& other) = delete;

        bool checkStep(IVector* step);

        unsigned int m_dim;
        unsigned int m_cardinality; // how many points in the compact's grid

        IVector const* m_begin;
        IVector const* m_end;
        IVector const* m_steps;

        QVector <IIteratorImpl*> iterators;
    };
}

ICompact::IIterator::IIterator(const ICompact *const compact, int pos, const IVector *const step){}

int ICompactImpl::getId() const
{
    return INTERFACE_0;
}

ICompact* ICompact::createCompact(const IVector *const begin, const IVector *const end, const IVector *const step)
{
    return ICompactImpl::createCompact(begin, end, step);
}

ICompact* ICompactImpl::createCompact(const IVector *const begin, const IVector *const end, const IVector *const step)
{
    if (!begin || !end)
    {
        ILog::report("ICompact.createCompact: constructor missed begin or end");
        return 0;
    }

    if (begin->getDim() != end->getDim())
    {
        ILog::report("ICompact.createCompact: dimension mismatch in createCompact");
        return 0;
    }

    int dim = begin->getDim();

    double* t_begin = new double[dim];
    double* t_end = new double[dim];
    if(!t_begin || !t_end)
    {
        delete[] t_begin;
        t_begin = 0;
        delete[] t_end;
        t_end = 0;

        ILog::report("ICompact.createCompact: not enough memory");
        return 0;
    }

    double b,e;
    for (unsigned int i = 0; i < dim; i++)
    {
        begin->getCoord(i, b);
        end->getCoord(i, e);
        t_begin[i] = (b < e)?b:e;
        t_end[i] = (b < e)?e:b;
    }

    IVector* try_begin = IVector::createVector(dim, t_begin);
    IVector* try_end = IVector::createVector(dim, t_end);

    delete[] t_begin;
    t_begin = 0;
    delete[] t_end;
    t_end = 0;

    if(!try_begin || !try_end)
    {
        delete try_begin;
        try_begin = 0;
        delete try_end;
        try_end = 0;

        ILog::report("ICompact.createCompact: not enough memory");
        return 0;
    }

    IVector* try_steps = 0;
    unsigned int card = 1;

    if (step) //non-default step
    {
        if (step->getDim() != begin->getDim() || step->getDim() != end->getDim() || begin->getDim() != end->getDim())
        {
            delete try_begin;
            try_begin = 0;
            delete try_end;
            try_end = 0;

            ILog::report("ICompact.createCompact: dimension mismatch in createCompact");
            return 0;
        }
        /*double b,e;
        for(unsigned int i = 0; i<end->getDim(); i++)
        {
            begin->getCoord(i,b);
            end->getCoord(i,e);
            /*if(b == e)
            {
                ILog::report("ICompact.createCompact: degenerate segment in compact construction");
                return 0;
            }
        }*/
        double d;

        for (unsigned int i = 0; i < dim; i++) // check if step data match the unsigned int
        {
            step->getCoord(i,d);
            if(d == 0)
            {
                delete try_begin;
                try_begin = 0;
                delete try_end;
                try_end = 0;

                ILog::report("ICompact.createCompact: zero step is unacceptable");
                return 0;
            }
            if((ABS(card) > (unsigned int)((double)UINT_MAX/ABS(d+1))))//Ivan's noting
            {
                delete try_begin;
                try_begin = 0;
                delete try_end;
                try_end = 0;

                ILog::report("ICompact.createCompact: there are too many vertices in the compact grid");
                return 0;
            }
            else
                card *= (unsigned int)ABS(d+1);//Ivan's noting
        }
        try_steps = step->clone();
        if(!try_steps)
        {
            ILog::report("ICompact.createCompact: not enough memory");
            return 0;
        }
        return new ICompactImpl(try_begin, try_end, try_steps, card);

    }
    else // default step
    {
        double gridStep = floor(pow((double)UINT_MAX,1./dim));
        double* tmp = new double[dim];
        if(!tmp)
        {
            delete try_begin;
            try_begin = 0;
            delete try_end;
            try_end = 0;

            ILog::report("ICompact.createCompact: not enough memory");
            return 0;
        }
        unsigned int card = 1;
        for(unsigned int i = 0; i < dim; i++)
        {
            tmp[i] = gridStep;
            card *= (unsigned int)ABS(gridStep);
        }
        try_steps = IVector::createVector(dim, tmp);

        delete[] tmp;
        tmp = 0;

        if(!try_steps)
        {
            delete try_begin;
            try_begin = 0;
            delete try_end;
            try_end = 0;

            ILog::report("ICompact.createCompact: not enough memory");
            return 0;
        }
        /*double b,e;
        for(unsigned int i = 0; i<end->getDim(); i++)
        {
            begin->getCoord(i,b);
            end->getCoord(i,e);
            if(b == e)
            {
                ILog::report("ICompact.createCompact: degenerate segment in compact construction");
                return 0;
            }
        }*/
        return new ICompactImpl(try_begin, try_end, try_steps, card);
    }
}

ICompactImpl::ICompactImpl(IVector* begin, IVector* end, IVector* step, unsigned int card):m_dim(begin->getDim()), m_begin(begin), m_end(end), m_steps(step), m_cardinality(card){}

ICompact* ICompactImpl::clone() const
{
    return ICompact::createCompact(m_begin, m_end, m_steps);
}

int ICompactImpl::deleteIterator(IIterator *pIter)
{
    if(!pIter)
    {
        ILog::report("ICompact.deleteIterator: got nullptr, expected ICompact::IITerator pointer");
        return ERR_WRONG_ARG;
    }

    if(this != dynamic_cast<IIteratorImpl*>(pIter)->getCompact())
    {
        ILog::report("ICompact.deleteIterator: tries to delete iterator which doesn't belong to this instance of ICompact");
        return ERR_WRONG_ARG;
    }
    int idx = iterators.indexOf(dynamic_cast<IIteratorImpl*>(pIter));
    if (idx == -1)
    {
        ILog::report("ICompact.deleteIterator: tries to delete iterator which doesn't belong to this instance of ICompact");
        return ERR_WRONG_ARG;
    }
    delete iterators[idx];
    iterators.remove(idx);

    return ERR_OK;
}


int ICompactImpl::getByIterator(const IIterator *pIter, IVector *&pItem) const
{
    if(!pIter)
    {
        ILog::report("ICompact.getByIterator: got nullptr, expected ICompact::IITerator pointer");
        return ERR_WRONG_ARG;
    }
    if(this != dynamic_cast<const IIteratorImpl*>(pIter)->getCompact())
    {
        ILog::report("ICompact.getByIterator: got iterator which doesn't belong to this instance of ICompact");
        return ERR_WRONG_ARG;
    }

    if(pItem)
        delete pItem;

    unsigned int pos = dynamic_cast<const IIteratorImpl*>(pIter)->getPos();
    IVector* tmp = 0;
    idxToVec(pos,tmp);

    pItem = tmp->clone();
    delete tmp;

    return ERR_OK;
}

int ICompactImpl::getNearestNeighbor(const IVector *vec, IVector *&nn) const
{
    if(!vec)
    {
        ILog::report("Icompact.getNearestNeighbor: got nullptrm expected IVector pointer");
        return ERR_WRONG_ARG;
    }
    if(vec->getDim() != m_dim)
    {
        ILog::report("Icompact.getNearestNeighbor: dimension mismatch in getNearestNeighbor");
        return ERR_WRONG_ARG;
    }
    bool contains;
    isContains(vec, contains);
    if(!contains)
    {
        ILog::report("Icompact.getNearestNeighbor: can't find nearest for vector outside the compact");
        return ERR_WRONG_ARG;
    }
    double* tmp = new double[m_dim];
    double x,b,e,s;
    for(unsigned int i = 0; i < m_dim; i++)
    {
        vec->getCoord(i,x);
        m_begin->getCoord(i,b);
        m_end->getCoord(i,e);
        m_steps->getCoord(i,s);
        unsigned int numStepBeforeVec = (unsigned int)floor(((x-b)/(e-b))*s);
        if(ABS(x-numStepBeforeVec*(e-b)/s - b) > ABS(x- (numStepBeforeVec+1)*(e-b)/s - b))
        {
            tmp[i] = (numStepBeforeVec+1)*(e-b)/s + b;
        }
        else
        {
            tmp[i] = numStepBeforeVec*(e-b)/s + b;
        }
    }

    if(nn)
        delete nn;

    nn = IVector::createVector(m_dim, tmp);
    delete[] tmp;
    tmp = 0;
    return ERR_OK;
}

ICompactImpl::IIterator* ICompactImpl::end(IVector const* const step)
{
    IIterator* it = new IIteratorImpl(this,m_cardinality-1,step);

    if(!checkStep(step))
    {
        delete it;
        return 0;
    }

    iterators.push_back(dynamic_cast<IIteratorImpl*>(it));
    return it;
}

ICompactImpl::IIterator* ICompactImpl::begin(IVector const* const step)
{
    IIterator* it = new IIteratorImpl(this,0,step);


    if(!checkStep(step))
    {
        delete it;
        return 0;
    }

    iterators.push_back(dynamic_cast<IIteratorImpl*>(it));
    return it;
}

int ICompactImpl::isContains(IVector const* const vec, bool& result) const
{
    if(!vec)
    {
        ILog::report("IComapct.isContains: got nullptrm expected IVector pointer");
        return ERR_WRONG_ARG;
    }
    if(vec->getDim() != m_dim)
    {
        ILog::report("IComapct.isContains: dimension mismatch in isContains");
        return ERR_WRONG_ARG;
    }
    result = false;
    double v,b,e;
    for(unsigned int i = 0; i < m_dim; i++)
    {
        m_begin->getCoord(i,b);
        m_end->getCoord(i,e);
        vec->getCoord(i,v);
        if (b > v || e < v)
            return ERR_OK;
    }
    result = true;
    return ERR_OK;
}

int ICompactImpl::isSubSet(ICompact const* const other) const//WHERE IS BOOL RESULT?????
{
    qt_assert("NOT IMPLEMENTED", __FILE__, __LINE__);
    return ERR_NOT_IMPLEMENTED;
}

ICompactImpl::~ICompactImpl()
{
    delete m_begin;
    delete m_end;
    delete m_steps;
    for(int i = 0; i < iterators.size(); i++)
    {
        delete iterators[i];
    }
    iterators.clear();
}

int ICompactImpl::IIteratorImpl::doStep()
{
    int lastError = ERR_OK;
    if(m_step)
    {
        IVector* posVec = 0;
        dynamic_cast<const ICompactImpl*>(m_com)->idxToVec(m_pos,posVec);
        IVector* tmp = IVector::add(m_step,posVec);
        if(!tmp)
        {
            ILog::report("IIterator.doStep: error while make step");
            return ERR_ANY_OTHER;
        }
        bool contains;
        lastError = dynamic_cast<const ICompactImpl*>(m_com)->isContains(tmp,contains);
        if(lastError!= ERR_OK)
        {
            ILog::report("IIterator.doStep: error while make step ");
            return lastError;
        }
        if(!contains)
        {
            ILog::report("IIterator.doStep: compact iterator out of range");
            return ERR_OUT_OF_RANGE;
        }
        IVector* ttmp = 0;
        lastError = m_com->getNearestNeighbor(tmp, ttmp);
        if(lastError!= ERR_OK)
        {
            ILog::report("IIterator.doStep: error while make step");
            return lastError;
        }
        dynamic_cast<const ICompactImpl*>(m_com)->vecToIdx(ttmp,m_pos);
        delete ttmp;
        delete tmp;
        delete posVec;
    }
    else
    {
        if(m_pos + 1 == dynamic_cast<const ICompactImpl*>(m_com)->m_cardinality)
        {
            ILog::report("IIterator.doStep: compact iterator out of range");
            return ERR_OUT_OF_RANGE;
        }
        else
            m_pos++;
    }
    return lastError;
}

int ICompactImpl::IIteratorImpl::setStep(IVector const* const step)
{
    if(!step)
    {
        ILog::report("ICompact.setStep: got nullptr, expected IVector pointer");
        return ERR_WRONG_ARG;
    }
    if(step->getDim() != dynamic_cast<const ICompactImpl*>(m_com)->m_dim)
    {
        ILog::report("ICompact.setStep: dimension mismatch in setStep");
        return ERR_WRONG_ARG;
    }

    if(!checkStep(step))
    {
        return ERR_INVALID_STEP;
    }

    IVector* tmp_step = step->clone();
    if(!tmp_step)
    {
        ILog::report("ICompact.IIterator.setStep: not enough memory");
        return ERR_MEMORY_ALLOCATION;
    }
    delete m_step;
    m_step = tmp_step;
}

ICompactImpl::IIteratorImpl::IIteratorImpl(ICompact const* const compact, int pos, IVector const* const step):ICompact::IIterator::IIterator(compact, pos, step)
{
    m_com = compact;
    m_pos = pos;
    if(step)
        m_step = step->clone();
    else
        m_step = 0;
}

ICompactImpl::IIteratorImpl::~IIteratorImpl()
{
    delete m_step;
}

void ICompactImpl::vecToIdx(IVector const* const vec, unsigned int& res) const
{
    if(!vec)
        return;

    double result = 0;
    double x,b,e,s;
    double* ind = new double[m_dim];
    for(unsigned int i = 0; i < m_dim; i++)
    {
        m_begin->getCoord(i,b);
        m_end->getCoord(i,e);
        m_steps->getCoord(i,s);
        vec->getCoord(i,x);
        ind[i] = (unsigned int) (((x - b)/(e - b))*s);

    }
    for (unsigned int i = m_dim - 1; i > 0; i--){
        m_steps->getCoord(i-1,s);
        result += ind[i];
        result *= s;
    }
    result += ind[0];
    res =(unsigned int)result;
    delete[] ind;
    ind = 0;
}

void ICompactImpl::idxToVec(unsigned int idx, IVector*& res) const
{
    double* vec = new double[m_dim];
    double b,e,s;
    for(unsigned int i = 0; i < m_dim; i++)
    {
        m_steps->getCoord(i,s);
        vec[i] = idx % (unsigned int)(s+1);
        idx /= (unsigned int)(s+1);
    }

    for(unsigned int i = 0; i < m_dim; i++)
    {
        m_begin->getCoord(i,b);
        m_end->getCoord(i,e);
        m_steps->getCoord(i,s);
        vec[i] *= (e - b)/(s+1);
    }

    for(unsigned int i = 0; i < m_dim; i++)
    {
        m_begin->getCoord(i,b);
        vec[i] += b;
    }
    res = IVector::createVector(m_dim, vec);
    delete[] vec;
    vec = 0;
}

bool ICompactImpl::checkStep(IVector* step)
{
    if(step)
    {
        IVector* it_pos = 0;
        int lastError = ERR_OK;
        if((lastError = getByIterator(it, it_pos)) != ERR_OK)
        {
            ILog::report("ICompact.begin: error while step verification");
            return false;
        }
        it->doStep();

        IVector* it_new_pos = 0;
        if((lastError = getByIterator(it, it_new_pos)) != ERR_OK)
        {
            ILog::report("ICompact.begin: error while step verification");
            return false;
        }

        int a, b, c;
        for(unsigned int i = 0; i < m_dim; i++)
        {
            it_pos->getCoord(i, a);
            it_new_pos->getCoord(i, b);
            if (a == b)
                c++;
        }
        if(c == m_dim)
        {
            ILog::report("ICompact.begin: given step not suitable for iteration");
            return false;
        }

    }
    return true;
}
