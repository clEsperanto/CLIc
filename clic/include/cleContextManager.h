#ifndef __cleContextManager_h
#define __cleContextManager_h

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
    
class ContextManager
{
private:
    cl_context context = nullptr;

public:
    ContextManager() = default;
    ~ContextManager() = default;
    ContextManager(cl_device_id);

    void CreateContext(cl_device_id&);

    cl_context& GetContext();
    cl_device_id GetDevice();
    cl_uint GetNumberOfDevices();

    std::string ToString() const;
};



} // namespace cle

#endif // __cleContextManager_h