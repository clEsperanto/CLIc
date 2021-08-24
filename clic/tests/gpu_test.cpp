
#include <random>

#include "clesperanto.hpp"
#include "cleGPU.hpp"


/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{
    cle::GPU* gpu = new cle::GPU();
    gpu->SelectDevice("GTX");
    gpu->SelectDevice();
    gpu->GetDeviceManager();
    gpu->GetContextManager();
    gpu->GetCommandQueueManager();
    gpu->GetPlatformManager();
    gpu->GetInfo();
    gpu->GetSelectedDeviceInfo();

    return EXIT_SUCCESS;
}