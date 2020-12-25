
#include "cleCommandQueueManager.h"
#include "utils.h"

namespace cle
{
    
CommandQueueManager::CommandQueueManager(cl_context context, cl_device_id device_id)
{
    this->CreateCommandQueue(context, device_id);
}

void CommandQueueManager::CreateCommandQueue(cl_context& context, cl_device_id &device_id)
{
    cl_int clError;        
#if CL_VERSION_2_0  // 2.0 or higher; see CL/cl_version.h
    this->command_queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &clError);
#else 
    this->command_queue = clCreateCommandQueue(context, device_id, 0, &clError);
#endif
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "CommandQueueManager : Fail to create command queue (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
}

cl_command_queue& CommandQueueManager::GetCommandQueue()
{
    return command_queue;
}

cl_device_id CommandQueueManager::GetDeviceId()
{
    cl_int clError;
    cl_device_id currentDevice;
    clError = clGetCommandQueueInfo(this->command_queue, CL_QUEUE_DEVICE, sizeof(currentDevice), &currentDevice, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "CommandQueueManager : Fail to get device from queue (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return currentDevice;
}

cl_context CommandQueueManager::GetContext()
{
    cl_int clError;
    cl_context currentContext;
    clError = clGetCommandQueueInfo(this->command_queue, CL_QUEUE_CONTEXT, sizeof(currentContext), &currentContext, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "CommandQueueManager : Fail to get context from queue (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return currentContext;
}

std::string CommandQueueManager::ToString() const
{
    cl_int clError;
    cl_uint ref_cnt;
    cl_device_id device;
    cl_context context;
    clError = clGetCommandQueueInfo(this->command_queue, CL_QUEUE_CONTEXT, sizeof(context), &context, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "CommandQueueManager : Fail to print context from queue (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    clError = clGetCommandQueueInfo(this->command_queue, CL_QUEUE_DEVICE, sizeof(device), &device, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "CommandQueueManager : Fail to print context from queue (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    clError = clGetCommandQueueInfo(this->command_queue, CL_QUEUE_REFERENCE_COUNT, sizeof(ref_cnt), &ref_cnt, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "CommandQueueManager : Fail to print reference from queue (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    std::string str = "CommandQueue(refCount=" + std::to_string(ref_cnt) + 
                    //   ", deviceId=" + std::to_string(device) + 
                    //   ", context=" + std::to_string(context) + 
                      ")";
    return str;
}


} // namespace cle

