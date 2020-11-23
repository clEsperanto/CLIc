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

};

} // namespace cle

#endif //__clgpu_h