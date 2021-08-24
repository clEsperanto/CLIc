
#include "clePlatformManager.hpp"

#include <iostream>

namespace cle
{

PlatformManager::PlatformManager()
{
    try
    {
        cl::Platform::get(&m_PlatformList);
    }
    catch(cl::Error& e)
    {
        std::cerr << "PlatformManager : Fail to get platforms ..." << std::endl;
        std::cerr << "\tException caught! " << e.what() << " error code " << e.err() << std::endl;
    }
}

PlatformManager::~PlatformManager()
{
    if(!m_PlatformList.empty())
    {
        m_PlatformList.clear();
    }
}

std::vector<cl::Platform> PlatformManager::GetPlatforms()
{
    return this->m_PlatformList;
}

cl::Platform PlatformManager::GetPlatform(int idx)
{
    return this->m_PlatformList[idx];
}

cl::Platform PlatformManager::GetPlatform(const char* str)
{
    if(!m_PlatformList.empty())
    {
        for( auto ite = this->m_PlatformList.begin(); ite != this->m_PlatformList.end(); ++ite)
        {
            if( ite->getInfo<CL_PLATFORM_NAME>().find(str) == 0 )
            {
                return *ite;
            }
        }
        return this->m_PlatformList.front();
    }
}


std::string PlatformManager::GetInfo()
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

