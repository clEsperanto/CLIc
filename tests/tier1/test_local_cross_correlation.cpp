#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestLocalCrossCorrelation : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 1> output;
  std::array<float, 3 * 3 * 1> input = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
  std::array<float, 3 * 3 * 1> kernel = { 0, 1, 0, 1, 2, 1, 0, 1, 0 };
  std::array<float, 3 * 3 * 1> valid = { 0, 0.35355344, 0, 0.35355344, 0.7071069, 0.35355344, 0, 0.35355344, 0 };
};

TEST_P(TestLocalCrossCorrelation, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_kernel = cle::Array::create(gpu_input);
  gpu_input->writeFrom(input.data());
  gpu_kernel->writeFrom(kernel.data());

  auto gpu_output = cle::tier1::local_cross_correlation_func(device, gpu_input, gpu_kernel, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.0001);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLocalCrossCorrelation, ::testing::ValuesIn(getParameters()));
