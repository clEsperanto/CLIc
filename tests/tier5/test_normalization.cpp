#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestNormalize : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                   backend;
  cle::Device::Pointer          device;
  std::array<float, 11 * 1 * 1> input = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  std::array<float, 11 * 1 * 1> valid = { 0, 0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, 1 };
  std::array<float, 11 * 1 * 1> output;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestNormalize, execute)
{
  auto gpu_input = cle::Array::create(11, 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::normalize_func(device, gpu_input, nullptr, 10, 90);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestNormalize, ::testing::ValuesIn(getParameters()));
