#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestClip : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 2 * 2 * 1> output;
  std::array<float, 2 * 2 * 1> input = { 0, 1, 2, 3 };
};

TEST_P(TestClip, executeMinMax)
{
  std::array<float, 2 * 2 * 1> valid = { 1, 1, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::clip_func(device, gpu_input, nullptr, 1, 2);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestClip, ::testing::ValuesIn(getParameters()));
