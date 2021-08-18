
#include "cleCommandQueueManager.hpp"

namespace cle
{
    
CommandQueueManager::CommandQueueManager(cl::Context context, cl::Device device) : m_CommandQueue(context, device)
{}

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

