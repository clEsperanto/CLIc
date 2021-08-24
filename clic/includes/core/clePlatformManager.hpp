#ifndef __clePlatformManager_hpp
#define __clePlatformManager_hpp

#include "clic.hpp"

#include <vector>

namespace cle
{
    
class PlatformManager
{
private:
    std::vector<cl::Platform> m_PlatformList;

public:
    PlatformManager();
    ~PlatformManager();

    std::vector<cl::Platform> GetPlatforms();
    cl::Platform GetPlatform(int =0);
    cl::Platform GetPlatform(const char*);

    std::string GetInfo();

};

} // namespace cle

#endif //__clePlatformManager_hpp