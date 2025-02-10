#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestclFFT : public ::testing::TestWithParam<std::string>
{
protected:

};

TEST_P(TestclFFT, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::fft::fft_demo(device);
}

std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestclFFT, ::testing::ValuesIn(getParameters()));
