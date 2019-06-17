#include "IBrockerImpl.h"
#include "IProblem.h"
#include "ISolver.h"
#include "ILog.h"

// returns interface implementation ID
int IBrockerImpl::getId() const{
   return INTERFACE_0;
}

// @param type - type of data (Problem or Solver)
// @param data - pointer to data
// creates IBrockerImpl instance
IBrockerImpl::IBrockerImpl(Type type, void* data): m_type(type), m_data(data){}

IBrocker* IBrockerImpl::createBrocker(Type type, data){
   return new IBrockerImpl(type, data);
}

// returns pointer to IBrocker instance
IBrocker* IBrocker::createBrocker(IBrocker::Type type){
    void* data;
    if(type == IBrocker::PROBLEM){
       data = new IProblem();
    } else if (type == IBrocker::SOLVER) {
       data = new ISolver();
    }
    if (data == nullptr){
        return nullptr;
    }
    return IBrockerImpl::createBrocker(type, data);
}

// @param type - type of data (Problem or Solver)
// returns 0 if types don't match, 1 otherwise
bool IBrockerImpl::canCastTo(Type type) const{
   return type == this->m_type;
}

// @param type - type of data (Problem or Solver)
// returns 0 if types can't be casted(don't match), otherwise returns pointer to data
void* IBrockerImpl::getInterfaceImpl(Type type) const{
   return this->m_data;
}

// clear memory of IBrocker instanse
int IBrockerImpl::release(){
    delete this->m_data; // необходимо сделать counter и проверять когда обратится в 0 -- тогда и чистить
    delete this;
    return ERR_OK;
}




