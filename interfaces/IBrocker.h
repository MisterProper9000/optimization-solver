#ifndef IBROCKER_H
#define IBROCKER_H

#include "error.h"
#include "SHARED_EXPORT.h"

class SHARED_EXPORT IBrocker
{
public:
    enum InterfaceTypes
    {
        INTERFACE_0,
        DIMENSION_INTERFACE_IMPL
    };

    enum Type
    {
        PROBLEM,
        SOLVER,
        UNKNOWN,
    };

    virtual int getId() const = 0;

    /*non default copyable*/
    IBrocker(const IBrocker& other) = delete;
    void operator=( const IBrocker& other) = delete;

    /*factories*/
    static IBrocker * createBrocker(Type type);

    virtual bool canCastTo(Type type) const = 0;
    virtual void* getInterfaceImpl(Type type) const = 0;

    virtual int release() = 0;

protected:
    /**/

    /*dtor*/
    virtual ~IBrocker() = default;


};

#endif // IBROCKER_H
