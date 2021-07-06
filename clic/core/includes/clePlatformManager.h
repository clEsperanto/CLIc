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
    int m_PlatformId;

public:
    PlatformManager(){};
    PlatformManager(int);
    ~PlatformManager();

    cl::Platform GetPlatform(int=-1);
};

} // namespace cle

#endif //__clePlatformManager_h