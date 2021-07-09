
#include "clePlatformManager.h"

#include <iostream>

namespace cle
{

PlatformManager::PlatformManager(int platform_id)
{
    this->m_PlatformId = platform_id;
    cl::Platform::get(&m_PlatformList);
    if(m_PlatformList.size()==0)
    {
        std::cerr << "PlatformManager : No platform found, please check OpenCL installation" << std::endl;
    }
    else
    {
        // std::cout << "Using platform: " << this->m_PlatformList[this->m_PlatformId].getInfo<CL_PLATFORM_NAME>() << std::endl;
    }
}

PlatformManager::~PlatformManager()
{
    if(m_PlatformList.size()!=0)
    {
        m_PlatformList.clear();
    }
}

cl::Platform PlatformManager::GetPlatform(int platform_id)
{
    if (platform_id == -1)
    {
        return this->m_PlatformList[this->m_PlatformId];
    }
    else if (platform_id < this->m_PlatformList.size())
    {
        return this->m_PlatformList[platform_id];
    }
    else
    {
        std::cerr << "PlatformManager : wrong platform id. Return default platform." << std::endl;
        return this->m_PlatformList[this->m_PlatformId];
    }
}

} // namespace cle

