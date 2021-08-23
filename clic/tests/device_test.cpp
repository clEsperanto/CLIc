#include "cleDeviceManager.hpp"
#include "clePlatformManager.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    cle::PlatformManager platform_manager;

    {
    cle::DeviceManager * device_manager = new cle::DeviceManager();
    device_manager->GetInfo();
    device_manager->GetInfo(200);
    device_manager->GetAllInfo();
    delete device_manager;
    }

    {
    cle::DeviceManager * device_manager = new cle::DeviceManager(platform_manager.GetPlatforms());
    device_manager->GetInfo();
    device_manager->GetAllInfo();
    delete device_manager;
    }

    {
    cle::DeviceManager * device_manager = new cle::DeviceManager(platform_manager.GetPlatform("NVIDIA"));
    device_manager->GetInfo();
    device_manager->GetAllInfo();
    device_manager->SetDevice("GTX");
    device_manager->SetDevice(0);
    device_manager->GetDevice();
    device_manager->GetDevice(0);
    device_manager->GetDevices();
    delete device_manager;
    }

    return EXIT_SUCCESS;
}