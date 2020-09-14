/**
 * Author: Stephane Rigaud - @strigaud 
 */

#ifndef __cleKernel_h
#define __cleKernel_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <string>
#include <fstream>
#include <iostream>
#include <map>

#include "cleBuffer.h"
#include "cleGPU.h"
#include "cleConfig.h"

namespace cle
{

class Kernel
{
private:

    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel;

    const std::string filesep = "/";
    const std::string preambleFile = cle_preamble_folder + filesep + "preamble.cl";
    const std::string kernelFolder = cle_kernel_folder;    

protected:
    std::string dimensionality = "";
    std::string kernelName;
    std::map<std::string, Buffer> parameters;

    std::string LoadPreamble();
    std::string LoadSources();
    std::string LoadDefines();
    std::string DefineDimensionality(Buffer&);

    void CompileKernel();

public:
    Kernel(GPU&);
    ~Kernel(){};

    virtual void Execute(){};

    std::string GetKernelName();
    cl_kernel GetKernel();
    cl_program GetProgram();
    cl_device_id GetDevice();
    cl_context GetContext();
    cl_command_queue GetCommandQueue();
    
};

} // namespace cle

#endif // __cleKernel_h