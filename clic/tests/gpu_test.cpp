
#include <random>

#include "clesperanto.hpp"

#include "cleGPU.hpp"
#include "cleContextManager.hpp"
#include "cleCommandQueueManager.hpp"
#include "cleDeviceManager.hpp"
#include "clePlatformManager.hpp"



int platform_test()
{
    cle::PlatformManager platforms;
    if(platforms.GetPlatforms().empty())
    {
        return EXIT_FAILURE;
    }
    std::cout << platforms.GetInfo() << std::endl;
    return EXIT_SUCCESS;
}

int device_test()
{
    cle::PlatformManager platforms;
    cle::DeviceManager devices;
    cle::DeviceManager devices1(platforms.GetPlatforms());
    cle::DeviceManager devices2(platforms.GetPlatforms().front());
    try
    {
        devices1.SetDevice("GTXXXXXX");
        devices1.SetDevice("GTX");
        devices1.SetDevice(0);
        devices1.SetDevice(1000);
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    try
    {
        devices1.GetDevice();
        devices1.GetDevice(0);
        devices1.GetDevice(10);
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    if(devices1.GetDevices().empty())
    {
        return EXIT_FAILURE;
    }
    std::cout << devices1.GetInfo() << std::endl;
    std::cout << devices1.GetAllInfo() << std::endl;
    return EXIT_SUCCESS;
}

int context_test()
{
    cle::PlatformManager platforms;
    cle::DeviceManager devices(platforms.GetPlatforms());
    cle::ContextManager context;
    cle::ContextManager context1(devices.GetDevice());
    cle::ContextManager context2(devices.GetDevices());

    try
    {
        context2.GetContext();
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    if(context2.GetDevices().empty())
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int cq_test()
{
    cle::PlatformManager platforms;
    cle::DeviceManager devices(platforms.GetPlatforms());
    cle::ContextManager context(devices.GetDevice());
    cle::CommandQueueManager cq;
    cle::CommandQueueManager cq1(context.GetContext(), devices.GetDevices().front());
    
    try
    {
        cq1.GetCommandQueue();
        cq1.GetDevice();
        cq1.GetContext();
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int gpu_test()
{

    cle::GPU gpu;
    cle::GPU gpu2("GTX");
    
    try
    {
        gpu.SelectDevice("GTX");
        gpu.SelectDevice(0);
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }
    try
    {
        gpu.GetDeviceManager();
        gpu.GetContextManager();
        gpu.GetCommandQueueManager();
        gpu.GetPlatformManager();
    }
    catch(int err)
    {
        return EXIT_FAILURE;
    }

    gpu.GetInfo();
    gpu.GetSelectedDeviceInfo();
    
    return EXIT_SUCCESS;
}

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{

    if(platform_test())
    {
        return EXIT_FAILURE;
    }
    if(device_test())
    {
        return EXIT_FAILURE;
    }
    if(context_test())
    {
        return EXIT_FAILURE;
    }
    if(cq_test())
    {
        return EXIT_FAILURE;
    }
    if(gpu_test())
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}