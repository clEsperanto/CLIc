#ifndef __cleCommandQueueManager_hpp
#define __cleCommandQueueManager_hpp

#include "clic.hpp"

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

#endif // __cleCommandQueueManager_hpp