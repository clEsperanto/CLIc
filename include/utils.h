/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
*/


#ifndef __utils_h
#define __utils_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <string>
#include <iostream>

/**
 * save string into text file
 */ 
void write_kernel_source(std::string);

/**
 * OpenCL error code into string function
 */ 
const char *getOpenCLErrorString(cl_int);


cl_mem CreateBuffer(size_t, cl_context);

template<class T>
bool WriteBuffer(cl_mem mem_obj, T* arr, size_t bitSize, cl_command_queue command_queue)
{
    cl_int clError = clEnqueueWriteBuffer(command_queue, mem_obj, CL_TRUE, 0, bitSize, arr, 0, nullptr, nullptr);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Push error! fail to write buffer in push() " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    return true;
}

template<class T>
T* ReadBuffer(cl_mem mem_obj, size_t bitSize, cl_command_queue command_queue)
{
    size_t size = bitSize / sizeof(T);
    T* arr = new T[size];
    cl_int clError = clEnqueueReadBuffer(command_queue, mem_obj, CL_TRUE, 0, bitSize, arr, 0, NULL, NULL);
    if (clError != CL_SUCCESS)
    {
        std::cerr << "Pull error! fail to read buffer : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
    return arr;
}


#endif // __utils_h