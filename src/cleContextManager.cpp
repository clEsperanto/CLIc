
#include "cleContextManager.h"
#include "utils.h"

namespace cle
{
    
ContextManager::ContextManager(cl_device_id device_id)
{
    this->CreateContext(device_id);
}

void ContextManager::CreateContext(cl_device_id &device_id)
{
    cl_int clError;
    this->context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "ContextManager : Fail to create context (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
}

cl_context& ContextManager::GetContext()
{
    return this->context;
}

cl_device_id ContextManager::GetDevice()
{
    cl_device_id currentDevice;
    cl_int clError;
    clError = clGetContextInfo(this->context, CL_CONTEXT_DEVICES, sizeof(currentDevice), &currentDevice, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "ContextManager : Fail to get devices (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return currentDevice;
}

cl_uint ContextManager::GetNumberOfDevices()
{
    cl_int clError;
    cl_uint num_dev = 0;
    clError = clGetContextInfo(this->context, CL_CONTEXT_NUM_DEVICES, sizeof(num_dev), &num_dev, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "ContextManager : Fail to get number of devices (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return num_dev;
}

std::string ContextManager::ToString() const
{
    cl_int clError;
    cl_uint ref_cnt, num_dev;
    cl_device_id device_id;
    clError = clGetContextInfo(this->context, CL_CONTEXT_REFERENCE_COUNT, sizeof(ref_cnt), &ref_cnt, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Fail to get context reference count : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    clError = clGetContextInfo(this->context, CL_CONTEXT_NUM_DEVICES, sizeof(num_dev), &num_dev, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Fail to get context number of devices : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    clError = clGetContextInfo(this->context, CL_CONTEXT_DEVICES, sizeof(device_id), &device_id, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Fail to get device list from context : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    std::string str = "Context( refCount=" + std::to_string(ref_cnt) 
                    + ",#devices=" + std::to_string(num_dev) + ")";
                    // + ",deviceId=" + std::to_string(device_id) + ")";
    return str;
}


} // namespace cle

