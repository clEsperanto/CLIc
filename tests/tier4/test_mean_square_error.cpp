
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMeanSquareError : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  const std::array<float, 3 * 1 * 1> input1 = { 1, 2, 3 };
  const std::array<float, 3 * 1 * 1> input2 = { 4, 5, 7 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestMeanSquareError, execute)
{
  auto gpu_input1 = cle::Array::create(3, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto output = cle::tier4::mean_squared_error_func(device, gpu_input1, gpu_input2);
  EXPECT_NEAR(output, 11.333, 0.001);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMeanSquareError, ::testing::ValuesIn(getParameters()));
