
#include "clePlatformManager.h"

#include <iostream>

namespace cle
{

PlatformManager::PlatformManager()
{
    cl::Platform::get(&m_PlatformList);
    if(m_PlatformList.size()==0)
    {
        std::cerr << "PlatformManager : No platform found, please check OpenCL installation" << std::endl;
    }
}

PlatformManager::~PlatformManager()
{
    if(m_PlatformList.size()!=0)
    {
        m_PlatformList.clear();
    }
}


std::vector<cl::Platform> PlatformManager::GetPlatformList()
{
    return this->m_PlatformList;
}

cl::Platform PlatformManager::GetPlatform(int platform_id)
{
    if (platform_id < this->m_PlatformList.size())
    {
        return this->m_PlatformList[platform_id];
    }
    else
    {
        std::cerr << "PlatformManager : wrong platform id. Returning default platform." << std::endl;
        return this->m_PlatformList;
    }
}


std::string PlatformManager::PlatformListInfo()
{
    std::string out ("List of Platform available:\n");
    for( auto ite = this->m_PlatformList.begin(); ite != this->m_PlatformList.end(); ++ite)
    {
        out += "\t";
        out += "[" + std::to_string(ite - this->m_PlatformList.begin()) + "] - " + ite->getInfo<CL_PLATFORM_NAME>() + "\n";
    }
    return out;
}

} // namespace cle

