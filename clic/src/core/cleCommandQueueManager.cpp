
#include "cleCommandQueueManager.hpp"
#include <iostream>

namespace cle
{
    
CommandQueueManager::CommandQueueManager(cl::Context context, cl::Device device)
{
    try
    {
        m_CommandQueue = cl::CommandQueue(context, device);
    }
    catch(cl::Error& e)
    {
        std::cerr << "CommandQueueManager : Fail to create command queue from context and device." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

CommandQueueManager::~CommandQueueManager()
{}

cl::CommandQueue CommandQueueManager::GetCommandQueue()
{
    return this->m_CommandQueue;
}

cl::Device CommandQueueManager::GetDevice()
{
    cl::Device device;
    try 
    {
        device = this->m_CommandQueue.getInfo<CL_QUEUE_DEVICE>();
    }
    catch(const cl::Error& e)
    {
        std::cerr << "CommandQueueManager : Error getting device from CommandQueue." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
    return device;
}

cl::Context CommandQueueManager::GetContext()
{
    cl::Context context;
    try 
    {
        context = this->m_CommandQueue.getInfo<CL_QUEUE_CONTEXT>();
    }
    catch(const cl::Error& e)
    {
        std::cerr << "CommandQueueManager : Error getting context from CommandQueue." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " -> " << e.err() << '\n';
    }
    return context;
}

} // namespace cle

