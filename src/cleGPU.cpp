/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#include "cleGPU.h"
#include "utils.h"

namespace cle
{

GPU::GPU()
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
    
void GPU::InitialisePlatform()
{
    cl_uint ret_num_platforms;
    cl_int clError = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    if (clError != CL_SUCCESS)  
    {
        std::cerr << "Initialisation error! Could not get platform : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void GPU::InitialiseDevice()
{
    cl_uint ret_num_devices;
    cl_int clError = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not get device : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void GPU::CreateContext()
{
    cl_int clError;
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &clError);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not create context : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void GPU::CreateCommandQueue()
{
    cl_int clError;        
#if OpenCL_VERSION >= 2  // 2.0 or higher
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

cl_platform_id GPU::GetPlateform()
{
    return platform_id;
}

cl_device_id GPU::GetDevice()
{
    return device_id;
}

cl_context GPU::GetContext()
{
    return context;
}

cl_command_queue GPU::GetCommandQueue()
{
    return command_queue;
}

} // namespace cle
