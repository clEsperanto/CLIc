

#ifndef __cleGPU_h
#define __cleGPU_h

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

#include <CL/cl2.hpp>

#include "cleDeviceManager.h"
#include "cleContextManager.h"
#include "clePlatformManager.h"
#include "cleCommandQueueManager.h"

#include <map>

namespace cle
{
    
class GPU
{
private:

    PlatformManager m_PlatformManager;
    DeviceManager m_DeviceManager;
    ContextManager m_ContextManager;
    CommandQueueManager m_CommandQueueManager;

    std::map<size_t, cl::Program> m_ProgramList;

public:
    GPU();
    ~GPU() = default;

    DeviceManager GetDeviceManager();
    ContextManager GetContextManager();
    CommandQueueManager GetCommandQueueManager();
    PlatformManager GetPlatformManager();

    bool FindProgram(size_t);
    cl::Program GetProgram(size_t);
    void AddProgram(cl::Program, size_t);
};











} // namespace cle

#endif //__clgpu_h
