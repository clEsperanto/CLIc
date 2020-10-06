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

#include "cleBuffer.h"
#include "cleConfig.h"
#include "image.h"
#include "utils.h"

namespace cle
{
    
class GPU
{
private:
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;

protected:

    void InitialisePlatform();
    void InitialiseDevice();
    void CreateContext();
    void CreateCommandQueue();

public:
    GPU();
    ~GPU(){};

    cl_platform_id GetPlateform();
    cl_device_id GetDevice();
    cl_context GetContext();
    cl_command_queue GetCommandQueue();

};

} // namespace cle

#endif //__clgpu_h