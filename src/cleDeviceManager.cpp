
#include "cleDeviceManager.h"

#include "utils.h"

namespace cle
{

DeviceManager::DeviceManager(cl_platform_id platform)
{
    this->RequestDevice(platform);
}

void DeviceManager::RequestDevice(cl_platform_id& platform)
{
    cl_uint ret_num_devices;
    cl_int clError = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, &ret_num_devices);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "DeviceManager : Fail to request device (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
}

cl_platform_id DeviceManager::GetPlatform()
{
    cl_platform_id platform_id;
    cl_int clError = clGetDeviceInfo(this->device, CL_DEVICE_PLATFORM, sizeof(platform_id), &platform_id, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "DeviceManager : Fail to get platform (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    return platform_id;
}

cl_device_id& DeviceManager::GetDevice()
{
    return this->device;
}


std::string DeviceManager::ToString() const
{
    cl_int clError;
    cl_device_type device_type;
    cl_platform_id platform;
    char* name;
    clError = clGetDeviceInfo(this->device, CL_DEVICE_NAME, sizeof(name), &name, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "DeviceManager : Fail to get device name (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    clError = clGetDeviceInfo(this->device, CL_DEVICE_PLATFORM, sizeof(platform), &platform, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "DeviceManager : Fail to get device pltaform (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    clError = clGetDeviceInfo(this->device, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "DeviceManager : Fail to get device type (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    std::string str = "Device(name=";
                str += name; 
                str += ",type=" ;
                str += std::to_string(device_type); 
                // str += ",platform=" ;
                // str += std::to_string(platform);
                str += ")";
    return str;
}



} // namespace cle

