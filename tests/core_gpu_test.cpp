
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
    try
    {
      auto gpu = std::make_shared<cle::Processor>("TX");
    }
    catch (const std::exception & e)
    {
      std::cerr << "No GPU found with TX" << std::endl;
    }
  }

  // test device management methods
  {
    auto gpu = std::make_shared<cle::Processor>();
    gpu->SelectDevice();
    std::cout << gpu->GetDeviceInfo() << std::endl;
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
  }

  // test device compatible with double
  {
    auto gpu = std::make_shared<cle::Processor>();
    gpu->SelectDevice();
    std::cout << gpu->DoubleSupport() << std::endl;
  }

  // test devise support image
  {
    auto gpu = std::make_shared<cle::Processor>();
    gpu->SelectDevice();
    std::cout << gpu->ImageSupport() << std::endl;
  }

  return EXIT_SUCCESS;
}