

#include "cleGPU.h"
#include <iostream>

namespace cle
{

GPU::GPU() 
{
    this->m_PlatformManager = PlatformManager();

    this->m_DeviceManager = DeviceManager(this->m_PlatformManager.GetPlatformList());

    this->m_ContextManager = ContextManager(this->m_DeviceManager.GetDevice());

    this->m_CommandQueueManager = CommandQueueManager(this->m_ContextManager.GetContext(), 
                                                this->m_DeviceManager.GetDevice());
}

GPU::GPU(std::string name) 
{
    this->m_PlatformManager = PlatformManager();

    this->m_DeviceManager = DeviceManager(this->m_PlatformManager.GetPlatformList());
    this->m_DeviceManager.SetDevice(name);

    this->m_ContextManager = ContextManager(this->m_DeviceManager.GetDevice());

    this->m_CommandQueueManager = CommandQueueManager(this->m_ContextManager.GetContext(), 
                                                this->m_DeviceManager.GetDevice());
}

void GPU::SelectDevice(std::string name)
{
    this->m_DeviceManager.SetDevice(name);
    this->m_ContextManager = ContextManager(this->m_DeviceManager.GetDevice());
    this->m_CommandQueueManager = CommandQueueManager(this->m_ContextManager.GetContext(), 
                                                this->m_DeviceManager.GetDevice());
}

DeviceManager GPU::GetDeviceManager()
{
    return this->m_DeviceManager;
}

ContextManager GPU::GetContextManager()
{
    return this->m_ContextManager;
}

CommandQueueManager GPU::GetCommandQueueManager()
{
    return this->m_CommandQueueManager;
}

PlatformManager GPU::GetPlatformManager()
{
    return this->m_PlatformManager;
}

cl::Program GPU::GetProgram(size_t hash)
{
    auto it = m_ProgramList.find(hash);
    return it->second;
}

bool GPU::FindProgram(size_t hash)
{
    auto it = m_ProgramList.find(hash);
    return it != m_ProgramList.end();
}

void GPU::AddProgram(cl::Program program, size_t hash)
{
    m_ProgramList.insert({hash, program});
}

void GPU::GetInfo()
{
    std::cout << m_PlatformManager.PlatformListInfo() << std::endl;
    std::cout << m_DeviceManager.DeviceListInfo() << std::endl;
}

void GPU::GetSelectedDeviceInfo()
{
    std::cout << "Current Selected Device: " << std::endl;
    std::cout << m_DeviceManager.GetDeviceInfo() << std::endl;
}

} // namespace cle
