#include "cle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestTopHat : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 50, 50, 50, 0, 0, 50, 100, 50, 0, 0, 50, 50, 50, 0, 0, 0, 0, 0, 0 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestTopHat, executeBox)
{

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::top_hat_box_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 50);
}

TEST_P(TestTopHat, executeSphere)
{

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::top_hat_sphere_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 50);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestTopHat, ::testing::ValuesIn(getParameters()));
