#ifndef __cleCommandQueueManager_h
#define __cleCommandQueueManager_h

#ifndef CL_HPP_ENABLE_EXCEPTIONS
#   define CL_HPP_ENABLE_EXCEPTIONS
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#   define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#   define CL_HPP_MINIMUM_OPENCL_VERSION 120
#endif

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#ifdef __APPLE__
#   include <OpenCL/opencl.hpp>
#else
#   include <CL/cl2.hpp>
#endif

#include <vector>


namespace cle
{
    
class CommandQueueManager
{
private:
    cl::CommandQueue m_CommandQueue;

public:
    CommandQueueManager(){};
    CommandQueueManager(cl::Context, cl::Device);
    ~CommandQueueManager();

    cl::CommandQueue GetCommandQueue();
    cl::Device GetDevice();
    cl::Context GetContext();
};



} // namespace cle

#endif // __cleCommandQueueManager_h