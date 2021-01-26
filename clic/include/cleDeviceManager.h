#ifndef __cleDeviceManager_h
#define __cleDeviceManager_h

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

#   include <CL/cl2.hpp>

#include <vector>

namespace cle
{
    
class DeviceManager
{
private:
    std::vector<cl::Device> m_DeviceList;
    int m_DeviceId;

public:
    DeviceManager(){};
    DeviceManager(cl::Platform, int=0);
    ~DeviceManager();

    cl::Device GetDevice(int=-1);
    std::vector<cl::Device> GetDeviceList();
};

} // namespace cle

#endif //__cleDeviceManager_h