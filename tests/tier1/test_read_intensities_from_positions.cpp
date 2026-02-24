#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestReadIntensityFromPositions : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 1 * 1> output;
  std::array<float, 3 * 3 * 1> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  std::array<float, 3 * 2 * 1> list = { 0, 1, 0, 0, 2, 2 };
  std::array<float, 3 * 1 * 1> valid = { 1, 8, 7 };
};

TEST_P(TestReadIntensityFromPositions, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(3, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input1->writeFrom(input.data());
  gpu_input2->writeFrom(list.data());

  auto gpu_output = cle::tier1::read_values_from_positions_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestReadIntensityFromPositions, ::testing::ValuesIn(getParameters()));
