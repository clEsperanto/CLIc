#include "cle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestStandardDeviation : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid_box = { 0,     0, 0, 0,     0,     0,     0.314, 0.314, 0.314, 0, 0, 0.314, 0.314,
                                             0.314, 0, 0, 0.314, 0.314, 0.314, 0,     0,     0,     0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid_sphere = { 0, 0, 0, 0, 0, 0, 0, 0.4, 0, 0, 0, 0.4, 0.4, 0.4, 0, 0, 0, 0.4, 0, 0, 0, 0, 0, 0, 0 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestStandardDeviation, executeBox)
{
  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::standard_deviation_box_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_box[i], 0.001);
  }
}

TEST_P(TestStandardDeviation, executeSphere)
{
  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::standard_deviation_sphere_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_sphere[i], 0.0001);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStandardDeviation, ::testing::ValuesIn(getParameters()));
