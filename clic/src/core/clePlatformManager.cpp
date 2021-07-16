
#include "clePlatformManager.h"

#include <iostream>

namespace cle
{

PlatformManager::PlatformManager()
{
    cl::Platform::get(&m_PlatformList);
    if(m_PlatformList.empty())
    {
        std::cerr << "PlatformManager : No platform found, please check OpenCL installation" << std::endl;
    }
}

PlatformManager::~PlatformManager()
{
    if(m_PlatformList.empty())
    {
        m_PlatformList.clear();
    }
}

std::vector<cl::Platform> PlatformManager::GetPlatforms()
{
    return this->m_PlatformList;
}

std::string PlatformManager::PlatformsInfo()
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

