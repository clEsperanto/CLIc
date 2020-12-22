#ifndef __cleCommandQueueManager_h
#define __cleCommandQueueManager_h

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#ifdef __APPLE__
#   include <OpenCL/opencl.h>
#else
#   include <CL/cl.h>
#endif

#include <iostream>


namespace cle
{
    
class CommandQueueManager
{
private:
    cl_command_queue command_queue = nullptr;

public:
    CommandQueueManager() = default;
    ~CommandQueueManager() = default;
    CommandQueueManager(cl_context, cl_device_id);


    void CreateCommandQueue(cl_context&, cl_device_id&);
    cl_command_queue& GetCommandQueue();
    cl_device_id GetDeviceId();
    cl_context GetContext();

    std::string ToString() const;
};



} // namespace cle

#endif // __cleCommandQueueManager_h