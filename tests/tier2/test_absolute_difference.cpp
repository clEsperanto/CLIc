#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestAbsoluteDifference : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 1 * 1> output;
  std::array<float, 3 * 1 * 1> input1 = { 1, 5, 3 };
  std::array<float, 3 * 1 * 1> input2 = { 4, 2, 7 };
  std::array<float, 3 * 1 * 1> valid = { 3, 3, 4 };
};

TEST_P(TestAbsoluteDifference, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(3, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());
  auto gpu_output = cle::tier2::absolute_difference_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestAbsoluteDifference, ::testing::ValuesIn(getParameters()));
