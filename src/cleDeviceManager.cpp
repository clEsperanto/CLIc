
#include "cleDeviceManager.h"

namespace cle
{

DeviceManager::DeviceManager(/* args */)
{
    this->RequestPlatform();
    this->RequestDevice();
}

void DeviceManager::RequestPlatform()
{
    cl_uint ret_num_platforms;
    cl_int clError = clGetPlatformIDs(1, &platform, &ret_num_platforms);
    if (clError != CL_SUCCESS)  
    {
        std::cerr << "Initialisation error! Could not get platform : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

void DeviceManager::RequestDevice()
{
    cl_uint ret_num_devices;
    cl_int clError = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, &ret_num_devices);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "Initialisation error! Could not get device : " << getOpenCLErrorString(clError) << std::endl;
        throw clError;
    }
}

cl_platform_id DeviceManager::GetPlatform()
{
    return this->platform;
}

cl_device_id DeviceManager::GetDevice()
{
    return this->device;
}

} // namespace cle

