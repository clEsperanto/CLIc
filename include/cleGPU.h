/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __cleGPU_h
#define __cleGPU_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "cleDeviceManager.h"
#include "cleContextManager.h"
#include "clePlatformManager.h"
#include "cleCommandQueueManager.h"

#include <iostream>
#include "utils.h"


namespace cle
{
    
class GPU
{
private:

    DeviceManager device_manager;
    ContextManager context_manager;
    CommandQueueManager commandqueue_manager;
    PlatformManager platform_manager;

public:
    GPU();
    ~GPU() = default;

    DeviceManager& GetDeviceManager();
    ContextManager& GetContextManager();
    CommandQueueManager& GetCommandQueueManager();
    PlatformManager& GetPlatformManager();

    std::string ToString() const;

    template<class T>
    friend cl_mem CreateBuffer(size_t, GPU&);
    template<class T>
    friend bool WriteBuffer(cl_mem, T*, size_t, GPU&);
    template<class T>
    friend T* ReadBuffer(cl_mem, size_t, GPU&);
};


template<class T>
cl_mem CreateBuffer(size_t size, GPU& gpu)
{
    cl_int clError;
    cl_mem mem_obj = clCreateBuffer(gpu.GetContextManager().GetContext(), CL_MEM_READ_WRITE, size * sizeof(T), nullptr, &clError);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Create Buffer : fail to create buffer space (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return mem_obj;
}

template<class T>
bool WriteBuffer(cl_mem mem_obj, T* arr, size_t bitSize, GPU& gpu)
{
    cl_int clError = clEnqueueWriteBuffer(gpu.GetCommandQueueManager().GetCommandQueue(), mem_obj, CL_TRUE, 0, bitSize, arr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Write Buffer : fail to write buffer space (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return true;
}

template<class T>
T* ReadBuffer(cl_mem mem_obj, size_t bitSize, GPU& gpu)
{
    size_t size = bitSize / sizeof(T);
    T* arr = new T[size];
    cl_int clError = clEnqueueReadBuffer(gpu.GetCommandQueueManager().GetCommandQueue(), mem_obj, CL_TRUE, 0, bitSize, arr, 0, NULL, NULL);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Read Buffer : fail to read buffer space (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return arr;
}


} // namespace cle

#endif //__clgpu_h