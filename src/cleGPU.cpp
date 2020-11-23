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
        platform_manager = PlatformManager();
    }
    catch(cl_int clError)
    {
        std::cerr << "GPU : Fail to initialise platform manager (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    try
    {
        device_manager = DeviceManager(this->platform_manager.GetPlatform());
    }
    catch(cl_int clError)
    {
        std::cerr << "GPU : Fail to initialise device manager (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    try
    {
        context_manager = ContextManager(this->device_manager.GetDevice());
    }
    catch(cl_int clError)
    {
        std::cerr << "GPU : Fail to initialise context manager (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    try
    {
        commandqueue_manager = CommandQueueManager(this->context_manager.GetContext(), 
                                                   this->context_manager.GetDevice());
    }
    catch(cl_int clError)
    {
        std::cerr << "GPU : Fail to initialise command queue manager (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
}
    
DeviceManager& GPU::GetDeviceManager()
{
    return this->device_manager;
}

ContextManager& GPU::GetContextManager()
{
    return this->context_manager;
}

CommandQueueManager& GPU::GetCommandQueueManager()
{
    return this->commandqueue_manager;
}

PlatformManager& GPU::GetPlatformManager()
{
    return this->platform_manager;
}

std::string GPU::ToString() const
{
    std::string str = "Current GPU state: \n";
    str += "\t" + platform_manager.ToString();
    str += "\t" + device_manager.ToString();
    str += "\t" + context_manager.ToString();
    str += "\t" + commandqueue_manager.ToString();
    return str;
}


} // namespace cle
