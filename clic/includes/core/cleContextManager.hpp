#ifndef __cleContextManager_hpp
#define __cleContextManager_hpp


#include "clic.hpp"

#include <vector>


namespace cle
{
    
class ContextManager
{
private:
    cl::Context m_Context;

public:
    ContextManager() =default;
    ContextManager(cl::Device);
    ContextManager(std::vector<cl::Device>);
    ~ContextManager();

    cl::Context GetContext();
    std::vector<cl::Device> GetDevices();
};

} // namespace cle

#endif // __cleContextManager_hpp