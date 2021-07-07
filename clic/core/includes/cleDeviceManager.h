#ifndef __cleDeviceManager_h
#define __cleDeviceManager_h

#include "clic.h"

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