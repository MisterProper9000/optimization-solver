#ifndef IBROCKER_H
#define IBROCKER_H

class IBrocker
{
public:
    enum Type
    {
        PROBLEM,
        SOLVER,
        UNKNOWN,
    };

    /*non default copyable*/
    IBrocker(const IBrocker& other) = delete;
    void operator=( const IBrocker& other) = delete;

    /*factories*/
    static IBrocker * createBrocker(Type type);

    virtual bool canCastTo(Type type) = 0;
    virtual void* getInterfaceImpl(Type type) = 0;

    virtual int release() = 0;

protected:
    /**/

    /*dtor*/
    virtual ~IBrocker() = default;


};

#endif // IBROCKER_H
