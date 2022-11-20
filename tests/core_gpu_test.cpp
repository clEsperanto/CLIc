
#include <iostream>
#include <memory>
#include <vector>

#include "cleProcessor.hpp"

/**
 * Main test function
 *
 */
int
main(int argc, char ** argv)
{

  // test default constructor
  {
    auto gpu = std::make_shared<cle::Processor>();
  }

  // test guided constructor for gpu
  {
    auto gpu = std::make_shared<cle::Processor>("GTX");
  }

  // test device management methods
  {
    auto gpu = std::make_shared<cle::Processor>();
    gpu->SelectDevice("GTX");
  }

  // test information methods
  {
    auto device_list = cle::Processor::ListAvailableDevices();
    std::cout << "[";
    for (auto it = device_list.begin(); it != device_list.end(); it++)
    {
      std::cout << *it;
      if ((it + 1) != device_list.end())
      {
        std::cout << " ; ";
      }
    }
    std::cout << "]" << std::endl;
    auto gpu = std::make_shared<cle::Processor>();
    gpu->SelectDevice(device_list.front());
  }

  return EXIT_SUCCESS;
}