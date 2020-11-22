#ifndef __cleDeviceManager_h
#define __cleDeviceManager_h

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <iostream>



namespace cle
{
    
class DeviceManager
{
private:
    cl_device_id device = nullptr;

public:
    DeviceManager() = default;
    ~DeviceManager() = default;
    DeviceManager(cl_platform_id);

    void RequestDevice(cl_platform_id&);

    cl_platform_id GetPlatform();
    cl_device_id& GetDevice();

    std::string ToString() const;
};

} // namespace cle

#endif //__cleDeviceManager_h