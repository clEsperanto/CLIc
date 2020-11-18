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
        device_manager = DeviceManager();
        context_manager = ContextManager(this->device_manager.GetDevice());
    }
    catch(cl_int clError)
    {
        std::cerr << "OCL error caught during initialisation! " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}
    
DeviceManager GPU::GetDeviceManager()
{
    return device_manager;
}

ContextManager GPU::GetContextManager()
{
    return context_manager;
}

} // namespace cle
