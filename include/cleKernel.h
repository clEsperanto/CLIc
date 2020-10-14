/*  CLIc - version 0.1 - Copyright 2020 Stéphane Rigaud, Robert Haase,
*   Institut Pasteur Paris, Max Planck Institute for Molecular Cell Biology and Genetics Dresden
*
*   CLIc is part of the clEsperanto project http://clesperanto.net 
*
*   This file is subject to the terms and conditions defined in
*   file 'LICENSE.txt', which is part of this source code package.
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
    const std::string preambleFile = PREAMBLE_FILE;
    const std::string kernelFolder = KERNELS_PATH;    

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