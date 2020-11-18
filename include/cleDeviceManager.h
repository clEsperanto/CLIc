#ifndef __cleDeviceManager_h
#define __cleDeviceManager_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "utils.h"
#include <iostream>

namespace cle
{
    
class DeviceManager
{
private:
    cl_device_id device = nullptr;
    cl_platform_id platform = nullptr;

protected:
    void RequestPlatform();
    void RequestDevice();

public:
    DeviceManager(/* args */);
    ~DeviceManager() = default;

    cl_platform_id GetPlatform();
    cl_device_id GetDevice();
};

} // namespace cle

#endif //__cleDeviceManager_h