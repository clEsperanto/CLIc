
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
        std::cerr << "ContextManager : Error creating context with provided device." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
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
        std::cerr << "ContextManager : Error creating context with provided devices." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
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
    std::vector<cl::Device> output;
    try 
    {
        output = this->m_Context.getInfo<CL_CONTEXT_DEVICES>();
    }
    catch(const cl::Error& e)
    {
        std::cerr << "ContextManager : Error getting device from context." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
    return output;
}

} // namespace cle

