#ifndef __clePlatformManager_h
#define __clePlatformManager_h

#include "clic.h"

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

    std::string PlatformsInfo();

};

} // namespace cle

#endif //__clePlatformManager_h