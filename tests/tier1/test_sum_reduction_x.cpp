#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestSumReductionX : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 1 * 1>  output;
  std::array<float, 12 * 1 * 1> input = { 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0 };
  std::array<float, 3 * 1 * 1>  valid = { 2.0, 2.0, 1.0 };
};

TEST_P(TestSumReductionX, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(12, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::sum_reduction_x_func(device, gpu_input, nullptr, 4);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSumReductionX, ::testing::ValuesIn(getParameters()));
