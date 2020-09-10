/**
 * Author: Stephane Rigaud - @strigaud 
 */

#include "clgpu.h"
#include "utils.h"

void clGPU::InitialisePlatform()
    {
        cl_uint ret_num_platforms;
        cl_int clError = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
        if (clError != CL_SUCCESS)  
        {
            std::cerr << "Initialisation error! Could not get platform : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
    }

void clGPU::InitialiseDevice()
{
    cl_uint ret_num_devices;
    cl_int clError = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not get device : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void clGPU::CreateContext()
{
    cl_int clError;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not create context : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void clGPU::CreateCommandQueue()
    {
        cl_int clError;        
#if OCL_MAJOR_VERSION == 2  // 2.0 or higher
        command_queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &clError);
#else 
        command_queue = clCreateCommandQueue(context, device_id, 0, &clError);
#endif
        if (clError != CL_SUCCESS) 
        {
            std::cerr << "Initialisation error! Could not create commande queue : " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
    }

void clGPU::Initialisation()
    {
        try
        {
            InitialisePlatform();
            InitialiseDevice();
            CreateContext();
            CreateCommandQueue();
        }
        catch(cl_int clError)
        {
            std::cerr << "OCL error caught during initialisation! " << getOpenCLErrorString(clError) << std::endl;
            throw clError;
        }
    }

cl_platform_id clGPU::GetPlateform()
{
    return platform_id;
}

cl_device_id clGPU::GetDevice()
{
    return device_id;
}

cl_context clGPU::GetContext()
{
    return context;
}

cl_command_queue clGPU::GetCommandQueue()
{
    return command_queue;
}