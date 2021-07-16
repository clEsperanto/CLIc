

#ifndef __cleGPU_h
#define __cleGPU_h

#include "clic.h"

#include "cleDeviceManager.h"
#include "cleContextManager.h"
#include "clePlatformManager.h"
#include "cleCommandQueueManager.h"

#include <map>
#include <iostream>

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
    GPU(std::string);
    ~GPU() = default;

    DeviceManager GetDeviceManager();
    ContextManager GetContextManager();
    CommandQueueManager GetCommandQueueManager();
    PlatformManager GetPlatformManager();

    void SelectDevice(std::string);
    void SelectDevice(int=0);

    bool FindProgram(size_t);
    cl::Program GetProgram(size_t);
    void AddProgram(cl::Program, size_t);

    void GetInfo();
    void GetSelectedDeviceInfo();
};











} // namespace cle

#endif //__clgpu_h
