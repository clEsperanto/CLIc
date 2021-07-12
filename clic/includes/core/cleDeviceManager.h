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
    int m_DeviceId = 0;

public:
    DeviceManager() = default;
    DeviceManager(cl::Platform);
    DeviceManager(std::vector<cl::Platform>);
    ~DeviceManager();

    void SetDevice(std::string);
    void SetDevice(int);

    cl::Device GetDevice();
    cl::Device GetDevice(int);

    std::vector<cl::Device> GetDeviceList();

    std::string GetDeviceInfo(int=-1);
    std::string DeviceListInfo();


};

} // namespace cle

#endif //__cleDeviceManager_h