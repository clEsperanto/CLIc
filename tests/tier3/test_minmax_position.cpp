#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMinMaxPosition : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 3 * 2> input = { 1, 2, 3, 10, 4, 16, 6, 11, 7, 8, 9, 12, 1, 2, 3, 13, 4, 0, 6, 16, 7, 8, 9, 15 };
};

TEST_P(TestMinMaxPosition, maxPosition)
{
  std::array<size_t, 3 * 1 * 1> valid = { 1, 1, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto output = cle::tier3::maximum_position_func(device, gpu_input);

  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestMinMaxPosition, minPosition)
{
  std::array<size_t, 3 * 1 * 1> valid = { 1, 1, 1 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto output = cle::tier3::minimum_position_func(device, gpu_input);

  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMinMaxPosition, ::testing::ValuesIn(getParameters()));
