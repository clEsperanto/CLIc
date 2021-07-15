
#include <random>

#include "CLE.h"

/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{

    // Platform Manager unitary test
    cle::PlatformManager Platforms;
    std::cout << "we have found " << Platforms.GetPlatformList().size() << " platform available" << std::endl;
    std::cout << Platforms.PlatformListInfo() << std::endl;

    // Device Manager unitary test
    cle::DeviceManager AllDevices(Platforms.GetPlatformList());
    std::cout << "we have found " << AllDevices.GetDeviceList().size() << " devices available" << std::endl;
    AllDevices.SetDevice(10); // setting a wrong device index
    AllDevices.SetDevice(0); // setting a correct device index
    AllDevices.SetDevice("BIG GPU FOR BIG BOY!"); // setting a wrong device name
    AllDevices.SetDevice("GTX"); // setting a correct device name
    cle::DeviceManager FirstDevices(Platforms.GetPlatformList().front());
    std::cout << FirstDevices.GetDeviceInfo() << std::endl;

    // Context Manager unitary test
    cle::ContextManager MultiDeviceContext(AllDevices.GetDeviceList());
    std::cout << "we have found " << MultiDeviceContext.GetDevices().size() << " devices in context" << std::endl;
    cle::ContextManager DeviceContext(AllDevices.GetDevice(0));

    // Command queue manager unitary test
    cle::CommandQueueManager CQ(DeviceContext.GetContext(), DeviceContext.GetDevices().front());
    cl::Device testDevice = CQ.GetDevice();
    cl::Context testContext = CQ.GetContext();
    
    // GPU test
    cle::GPU gpu;
    gpu.SelectDevice("GTX");
    gpu.GetSelectedDeviceInfo();
    std::cout << "we have found " << gpu.GetPlatformManager().GetPlatformList().size() << " platform available" << std::endl;

    cle::GPU gpu2("GTX");
    gpu2.GetInfo();

    return EXIT_SUCCESS;
}