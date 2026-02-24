#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestSumProjectionX : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 1 * 5 * 3>  output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 1 * 5 * 3>  valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(1));
    std::fill(valid.begin(), valid.end(), static_cast<float>(10));
  }
};

TEST_P(TestSumProjectionX, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::sum_x_projection_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSumProjectionX, ::testing::ValuesIn(getParameters()));
