#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestclFFT : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 2 * 2 * 1> input = {
    1, 2,
    3, 4,
  };
  // virtual void
  // SetUp()
  // {
  //   std::fill(input.begin(), input.end(), static_cast<float>(5));
  // }
};

TEST_P(TestclFFT, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);


  auto gpu_input = cle::Array::create(2, 2, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  cle::fft::fft_execute(gpu_input);
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
