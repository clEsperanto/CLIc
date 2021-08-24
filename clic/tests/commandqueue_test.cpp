#include "cleDeviceManager.hpp"
#include "clePlatformManager.hpp"
#include "cleContextManager.hpp"
#include "cleCommandQueueManager.hpp"


int main(int argc, char** argv)
{

    cle::PlatformManager* platform = new cle::PlatformManager();
    cle::DeviceManager* device = new cle::DeviceManager(platform->GetPlatform());
    cle::ContextManager* context = new cle::ContextManager(device->GetDevice());
    
    {
    cle::CommandQueueManager * cq = new cle::CommandQueueManager();
    cq->GetCommandQueue();
    cq->GetDevice();
    cq->GetContext();
    delete cq;
    }

    {
    cle::CommandQueueManager * cq = new cle::CommandQueueManager(context->GetContext(), device->GetDevice());
    cq->GetCommandQueue();
    cq->GetDevice();
    cq->GetContext();
    }
    
    return EXIT_SUCCESS;
}