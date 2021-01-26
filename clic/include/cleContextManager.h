#ifndef __cleContextManager_h
#define __cleContextManager_h


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
    
class ContextManager
{
private:
    cl::Context m_Context;

public:
    ContextManager(){};
    ContextManager(cl::Device);
    ContextManager(std::vector<cl::Device>);
    ~ContextManager();

    cl::Context GetContext();
    std::vector<cl::Device> GetDevices();
};

} // namespace cle

#endif // __cleContextManager_h