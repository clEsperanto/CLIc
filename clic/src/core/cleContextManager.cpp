
#include "cleContextManager.hpp"

namespace cle
{
    
ContextManager::ContextManager(cl::Device device) : m_Context(device)
{}

ContextManager::ContextManager(std::vector<cl::Device> devices) : m_Context(devices)
{}
    
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

