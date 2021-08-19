
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
        std::cerr << "Fail to create command queue ..." << std::endl;
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
    return this->m_CommandQueue.getInfo<CL_QUEUE_DEVICE>();
}

cl::Context CommandQueueManager::GetContext()
{
    return this->m_CommandQueue.getInfo<CL_QUEUE_CONTEXT>();
}

} // namespace cle

