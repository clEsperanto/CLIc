// Smoke test for an external project using CLIc: run one operation and check its result.
#include "cle.hpp"

#include <array>
#include <cmath>
#include <iostream>

auto
main() -> int
{
#if USE_OPENCL
  const std::string backend = "opencl";
#elif USE_CUDA
  const std::string backend = "cuda";
#elif USE_METAL
  const std::string backend = "metal";
#endif

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  if (device == nullptr)
  {
    std::cerr << "No " << backend << " device found" << std::endl;
    return 1;
  }
  device->setWaitToFinish(true);

  std::array<float, 6> input = { 0, 1, 2, 3, 4, 5 };
  std::array<float, 6> output = {};
  auto                 gpu_in = cle::Array::create(3, 2, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_in->writeFrom(input.data());
  auto gpu_out = cle::tier1::add_image_and_scalar_func(device, gpu_in, nullptr, 10);
  gpu_out->readTo(output.data());

  for (size_t i = 0; i < input.size(); ++i)
  {
    if (std::abs(output[i] - (input[i] + 10)) > 1e-6F)
    {
      std::cerr << "Wrong value at index " << i << ": " << output[i] << std::endl;
      return 1;
    }
  }
  std::cout << "CLIc package test passed on " << device->getName() << " (" << backend << ")" << std::endl;
  return 0;
}
