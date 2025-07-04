
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestPercentile : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestPercentile, executeInteger)
{
  const std::array<uint32_t, 10 * 1 * 1> input = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 1, 1, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto value = cle::tier4::percentile_func(device, gpu_input, 50);

  EXPECT_NEAR(value, 4, 0.01);
}

TEST_P(TestPercentile, executeFloat)
{
  const std::array<float, 10 * 1 * 1> input = { 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 1, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto value = cle::tier4::percentile_func(device, gpu_input, 50);

  EXPECT_NEAR(value, 0.4, 0.01);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestPercentile, ::testing::ValuesIn(getParameters()));
