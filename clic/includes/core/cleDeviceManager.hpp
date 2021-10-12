#ifndef __cleDeviceManager_hpp
#define __cleDeviceManager_hpp

#include "clic.hpp"

#include <vector>

namespace cle
{
    
class DeviceManager
{
private:
    std::vector<cl::Device> m_DeviceList;
    size_t m_DeviceId = 0;

public:
    DeviceManager() = default;
    DeviceManager(cl::Platform);
    DeviceManager(std::vector<cl::Platform>);
    ~DeviceManager();

    void SetDevice(std::string);
    void SetDevice(size_t);

    cl::Device GetDevice();
    cl::Device GetDevice(size_t);
    std::vector<cl::Device> GetDevices();

    std::string GetInfo(int=-1);
    std::string GetAllInfo();

};

} // namespace cle

#endif //__cleDeviceManager_hpp