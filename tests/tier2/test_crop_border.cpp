#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestCropBorder : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                  backend;
  cle::Device::Pointer         device;
  std::array<float, 4 * 4 * 1> input_2d = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1 };

  std::array<float, 4 * 4 * 4> input_3d = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1,

                                            0, 0, 0, 1, 0, 1, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1,

                                            0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1,

                                            0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestCropBorder, execute2D)
{
  std::array<float, 2 * 2 * 1> valid = { 0, 3, 0, 3 };
  std::array<float, 2 * 2 * 1> output;

  auto gpu_input = cle::Array::create(4, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2d.data());

  auto gpu_output = cle::tier2::crop_border_func(device, gpu_input, nullptr, 1);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestCropBorder, execute3D)
{
  std::array<float, 2 * 2 * 2> valid = { 1, 3, 0, 3, 0, 3, 0, 3 };
  std::array<float, 2 * 2 * 2> output;

  auto gpu_input = cle::Array::create(4, 4, 4, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_3d.data());

  auto gpu_output = cle::tier2::crop_border_func(device, gpu_input, nullptr, 1);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCropBorder, ::testing::ValuesIn(getParameters()));
