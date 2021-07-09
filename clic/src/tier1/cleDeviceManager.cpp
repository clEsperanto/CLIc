
#include "cleDeviceManager.h"

#include <iostream>

namespace cle
{

DeviceManager::DeviceManager(cl::Platform platform, int device_id)
{
    this->m_DeviceId = device_id;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &m_DeviceList);
    if (m_DeviceList.size() == 0)
    {
        std::cerr << "DeviceManager : No devices found." << std::endl;
    }
    else
    {
        // std::cout << "Using device: " << this->m_DeviceList[this->m_DeviceId].getInfo<CL_DEVICE_NAME>() << std::endl;
    }
}

DeviceManager::~DeviceManager()
{
    if (this->m_DeviceList.size() != 0)
    {
        this->m_DeviceList.clear();
    }
}


std::vector<cl::Device> DeviceManager::GetDeviceList()
{
    return this->m_DeviceList;
}


cl::Device DeviceManager::GetDevice(int device_id)
{
    if (device_id == -1)
    {
        return this->m_DeviceList[this->m_DeviceId];
    }
    else if (device_id < this->m_DeviceList.size())
    {
        return this->m_DeviceList[device_id];
    }
    else
    {
        std::cerr << "DeviceManager : wrong device id. Return default device." << std::endl;
        return this->m_DeviceList[this->m_DeviceId];
    }
}

} // namespace cle

