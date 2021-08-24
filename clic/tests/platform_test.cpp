
#include "clePlatformManager.hpp"
#include <iostream>


int main(int argc, char** argv)
{

    cle::PlatformManager* platform_manager = new cle::PlatformManager();
    platform_manager->GetPlatforms();
    platform_manager->GetPlatform(0);
    platform_manager->GetPlatform("NVIDIA");
    platform_manager->GetPlatform("riri-fifi-loulou");
    platform_manager->GetInfo();
    delete platform_manager;

    return EXIT_SUCCESS;
}