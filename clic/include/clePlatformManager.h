#ifndef __clePlatformManager_h
#define __clePlatformManager_h

#ifndef CL_HPP_ENABLE_EXCEPTIONS
#   define CL_HPP_ENABLE_EXCEPTIONS
#endif

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#   define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#ifndef CL_HPP_MINIMUM_OPENCL_VERSION
#   define CL_HPP_MINIMUM_OPENCL_VERSION 120
#endif

#ifndef CL_TARGET_OPENCL_VERSION
#  define CL_TARGET_OPENCL_VERSION 120
#endif

#ifdef __APPLE__
#   include <OpenCL/opencl.hpp>
#else
#   include <CL/cl2.hpp>
#endif

#include <vector>

namespace cle
{
    
class PlatformManager
{
private:
    std::vector<cl::Platform> m_PlatformList;
    int m_PlatformId;

public:
    PlatformManager(){};
    PlatformManager(int);
    ~PlatformManager();

    cl::Platform GetPlatform(int=-1);
};

} // namespace cle

#endif //__clePlatformManager_h