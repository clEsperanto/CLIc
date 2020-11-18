#ifndef __cleContextManager_h
#define __cleContextManager_h


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "utils.h"
#include <iostream>

namespace cle
{
    
class ContextManager
{
private:
    cl_context context = nullptr;
    cl_command_queue command_queue = nullptr;

protected:
    void CreateContext(cl_device_id);
    void CreateCommandQueue(cl_device_id);

public:
    ContextManager() = default;
    ContextManager(cl_device_id);
    ~ContextManager() = default;

    cl_context GetContext();
    cl_command_queue GetCommandQueue();
};



} // namespace cle

#endif // __cleContextManager_h