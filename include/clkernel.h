/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __clkernel_h
#define __clkernel_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "clbuffer.h"
#include "clgpu.h"

class clKernel
{
private:

    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;

    const std::string filesep = "/";
    const std::string preambleFile = CLP_PATH + filesep + "preamble.cl";
    const std::string kernelFolder = CLI_PATH;    

protected:
    std::string dimensionality = "";
    std::string kernelName;
    std::map<std::string, clBuffer> parameters;

    cl_device_id GetDevice();
    cl_context GetContext();
    cl_command_queue GetCommandQueue();

    std::string LoadPreamble();
    std::string LoadSources();
    std::string LoadDefines();
    std::string DefineDimensionality(clBuffer&);

    void CompileKernel();

public:
    clKernel(clGPU&);
    ~clKernel(){};

    virtual void Execute(){};

    std::string GetKernelName();
    cl_kernel GetKernel();
    cl_program GetProgram();
    
};


#endif // __clkernel_h