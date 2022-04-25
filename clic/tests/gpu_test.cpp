
#include <random>
#include <memory>
#include <vector>

#include "cleGPU.hpp"


/**
 * Main test function
 *
 */
int main(int argc, char **argv)
{

    // test default constructor
    {
        auto gpu = std::make_shared<cle::GPU>();
    }

    // test guided constructor for gpu
    {
        auto gpu = std::make_shared<cle::GPU>("GTX", "gpu");
    }

    // test guided constructor for gpu
    {
        auto gpu = std::make_shared<cle::GPU>("", "cpu");
    }

    // test device management methods
    {
        auto gpu = std::make_shared<cle::GPU>();
        gpu->SelectDevice("GTX");
    }

    // test information methods
    {
        auto gpu = std::make_shared<cle::GPU>();
        auto device_list = gpu->ListAvailableDevices();
        std::cout << "[";
        for (auto it = device_list.begin(); it != device_list.end(); ++it)
        {
            std::cout << *it;
            if ((it+1) != device_list.end())
            {
                std::cout << " ; ";
            } 
        }
        std::cout << "]\n";
        std::cout << gpu->Name() << std::endl;
        std::cout << gpu->Info() << std::endl;
        std::cout << std::to_string(gpu->Score()) << std::endl;
    }

    return EXIT_SUCCESS;
}