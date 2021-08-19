
#include "cleContextManager.hpp"
#include <iostream>

namespace cle
{
    
ContextManager::ContextManager(cl::Device device)
{
    try 
    {
        m_Context = cl::Context(device);
    }
    catch(const cl::Error& e)
    {
        std::cout << "Error creating context " << std::endl;
        std::cout << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
    
}

ContextManager::ContextManager(std::vector<cl::Device> devices)
{
    try 
    {
        m_Context = cl::Context(devices);
    }
    catch(const cl::Error& e)
    {
        std::cout << "Error creating context " << std::endl;
        std::cout << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
}
    
ContextManager::~ContextManager()
{}

cl::Context ContextManager::GetContext()
{
    return this->m_Context;
}

std::vector<cl::Device> ContextManager::GetDevices()
{
    return this->m_Context.getInfo<CL_CONTEXT_DEVICES>();
}

} // namespace cle

