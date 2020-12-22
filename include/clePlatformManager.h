#ifndef __clePlatformManager_h
#define __clePlatformManager_h

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#ifdef __APPLE__
#   include <OpenCL/opencl.h>
#else
#   include <CL/cl.h>
#endif

#include <iostream>

namespace cle
{
    
class PlatformManager
{
private:
    cl_platform_id platform = nullptr;

public:
    PlatformManager();
    ~PlatformManager() = default;

    void RequestPlatform();

    cl_platform_id& GetPlatform();

    std::string ToString() const;
};

} // namespace cle

#endif //__clePlatformManager_h