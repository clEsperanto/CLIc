#include "cleDeviceManager.hpp"
#include "clePlatformManager.hpp"
#include "cleContextManager.hpp"

int main(int argc, char **argv)
{
    cle::PlatformManager* platform = new cle::PlatformManager();
    cle::DeviceManager* device = new cle::DeviceManager(platform->GetPlatform());

    {
    cle::ContextManager* context = new cle::ContextManager();
    context->GetContext();
    context->GetDevices();
    delete context;
    }
    
    {
    cle::ContextManager* context = new cle::ContextManager(device->GetDevice());
    context->GetContext();
    context->GetDevices();
    delete context;
    }
    

    return EXIT_SUCCESS;
}