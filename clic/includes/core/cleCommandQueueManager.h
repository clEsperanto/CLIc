#ifndef __cleCommandQueueManager_h
#define __cleCommandQueueManager_h

#include "clic.h"

#include <vector>


namespace cle
{
    
class CommandQueueManager
{
private:
    cl::CommandQueue m_CommandQueue;

public:
    CommandQueueManager() =default;
    CommandQueueManager(cl::Context, cl::Device);
    ~CommandQueueManager();

    cl::CommandQueue GetCommandQueue();
    cl::Device GetDevice();
    cl::Context GetContext();
};



} // namespace cle

#endif // __cleCommandQueueManager_h