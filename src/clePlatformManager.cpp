
#include "clePlatformManager.h"


#include "utils.h"

namespace cle
{

PlatformManager::PlatformManager()
{
    this->RequestPlatform();
}

void PlatformManager::RequestPlatform()
{
    cl_uint ret_num_platforms;
    cl_int clError = clGetPlatformIDs(1, &platform, &ret_num_platforms);
    if (clError != CL_SUCCESS)  
    {
        std::cerr << "PlatformManager : Fail to request platform (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
}

cl_platform_id& PlatformManager::GetPlatform()
{
    return this->platform;
}

std::string PlatformManager::ToString() const
{
    cl_int clError;
    char* name, vendor;
    clError = clGetPlatformInfo(this->platform, CL_PLATFORM_NAME, sizeof(name), &name, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "PlatformManager : Fail to get platform name (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    clError = clGetPlatformInfo(this->platform, CL_PLATFORM_VENDOR, sizeof(vendor), &vendor, NULL);
    if (clError != CL_SUCCESS) 
    {
        std::cerr << "PlatformManager : Fail to get platform vendor (" << getOpenCLErrorString(clError) << ")" << std::endl;
        throw clError;
    }
    std::string str = "Platform(name=";
                str += name; 
                str += ",vendor=" ;
                str += vendor; 
                str += ")";
    return str;
}



} // namespace cle

