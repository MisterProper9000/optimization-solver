#ifndef IBROCKERIMPL_H
#define IBROCKERIMPL_H
#include "IBrocker.h"
#include "ILog.h"

namespace{
class IBrockerImpl: public virtual IBrocker{
public:

    static IBrocker * createBrocker(Type type);

    virtual int getId() const;
    virtual bool canCastTo(Type type) const;
    virtual void* getInterfaceImpl(Type type) const;
    virtual int release();

protected:
    IBrockerImpl(IBrocker::Type type, void * data);

    ~IBrockerImpl(){}

    IBrockerImpl(){}

private:

    IBrockerImpl(const IBrocker& other) = delete;
    void operator=(const IBrocker& other) = delete;

    Type m_type;
    void *m_data;
};
}
#endif // IBROCKERIMPL_H
