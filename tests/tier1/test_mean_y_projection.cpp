#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMeanProjectionY : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 1 * 2>  output;
  std::array<float, 10 * 10 * 2> input = {
    1, 2, 3, 4, 5,  6, 7, 8, 9,  10, 2, 3, 4, 5,  6, 7, 8, 9,  10, 1, 3, 4, 5,  6, 7, 8, 9,  10, 1, 2, 4, 5,  6, 7, 8, 9, 10, 1, 2, 3, 5,
    6, 7, 8, 9, 10, 1, 2, 3, 4,  6,  7, 8, 9, 10, 1, 2, 3, 4,  5,  7, 8, 9, 10, 1, 2, 3, 4,  5,  6, 8, 9, 10, 1, 2, 3, 4, 5,  6, 7, 9, 10,
    1, 2, 3, 4, 5,  6, 7, 8, 10, 1,  2, 3, 4, 5,  6, 7, 8, 9,  1,  2, 3, 4, 5,  6, 7, 8, 9,  10, 2, 3, 4, 5,  6, 7, 8, 9, 10, 1, 3, 4, 5,
    6, 7, 8, 9, 10, 1, 2, 4, 5,  6,  7, 8, 9, 10, 1, 2, 3, 5,  6,  7, 8, 9, 10, 1, 2, 3, 4,  6,  7, 8, 9, 10, 1, 2, 3, 4, 5,  7, 8, 9, 10,
    1, 2, 3, 4, 5,  6, 8, 9, 10, 1,  2, 3, 4, 5,  6, 7, 9, 10, 1,  2, 3, 4, 5,  6, 7, 8, 10, 1,  2, 3, 4, 5,  6, 7, 8, 9,
  };
  std::array<float, 10 * 1 * 2> valid = {
    5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5, 5.5
  };
};

TEST_P(TestMeanProjectionY, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 10, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::mean_y_projection_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMeanProjectionY, ::testing::ValuesIn(getParameters()));
