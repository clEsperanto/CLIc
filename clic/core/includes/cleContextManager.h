#ifndef __cleContextManager_h
#define __cleContextManager_h


#include "clic.h"

#include <vector>


namespace cle
{
    
class ContextManager
{
private:
    cl::Context m_Context;

public:
    ContextManager(){};
    ContextManager(cl::Device);
    ContextManager(std::vector<cl::Device>);
    ~ContextManager();

    cl::Context GetContext();
    std::vector<cl::Device> GetDevices();
};

} // namespace cle

#endif // __cleContextManager_h