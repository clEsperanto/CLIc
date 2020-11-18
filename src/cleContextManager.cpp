
#include "cleContextManager.h"

namespace cle
{
    
ContextManager::ContextManager(cl_device_id device_id)
{
    this->CreateContext(device_id);
    this->CreateCommandQueue(device_id);
}

void ContextManager::CreateContext(cl_device_id device_id)
{
    cl_int clError;
    this->context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not create context : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void ContextManager::CreateCommandQueue(cl_device_id device_id)
{
    cl_int clError;        
#if OpenCL_VERSION >= 2  // 2.0 or higher
    this->command_queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &clError);
#else 
    this->command_queue = clCreateCommandQueue(context, device_id, 0, &clError);
#endif
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not create commande queue : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

cl_context ContextManager::GetContext()
{
    return context;
}

cl_command_queue ContextManager::GetCommandQueue()
{
    return command_queue;
}


} // namespace cle

