#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestConvolve : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 4 * 1> output;
  std::array<float, 3 * 3 * 1> kernel = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
  std::array<float, 4 * 4 * 1> input = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1 };
  std::array<float, 4 * 4 * 1> valid = { 3, 1, 0, 0, 1, 4, 0, 0, 0, 0, 4, 1, 0, 0, 1, 4 };
};

TEST_P(TestConvolve, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_kernel = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_kernel->writeFrom(kernel.data());

  auto gpu_output = cle::tier1::convolve_func(device, gpu_input, gpu_kernel, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestConvolve, ::testing::ValuesIn(getParameters()));
