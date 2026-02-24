#include "cle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestClosing : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<float, 6 * 6 * 2> output;
  std::array<float, 6 * 6 * 2> input = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0,
                                         0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                         1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 2, 0, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0 };

  std::array<float, 6 * 6 * 2> valid_box = { 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2,
                                             1, 0, 0, 0, 2, 2, 3, 0, 0, 0, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                             1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 0, 0, 0, 2, 2, 3, 0, 0, 0, 2, 2 };

  std::array<float, 6 * 6 * 2> valid_sphere = { 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 2,
                                                1, 1, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                                1, 1, 1, 1, 2, 0, 1, 1, 1, 1, 2, 2, 1, 1, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestClosing, executeBox)
{
  auto gpu_input = cle::Array::create(6, 6, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::closing_box_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestClosing, executeSphere)
{
  auto gpu_input = cle::Array::create(6, 6, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::closing_sphere_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_sphere[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestClosing, ::testing::ValuesIn(getParameters()));
