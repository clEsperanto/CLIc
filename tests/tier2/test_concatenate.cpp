#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestConcatenate : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<float, 2 * 2 * 2> output;
  std::array<float, 2 * 2 * 1> input1 = { 1, 1, 1, 1 };
  std::array<float, 2 * 2 * 1> input2 = { 2, 2, 2, 2 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestConcatenate, alongX_withDst)
{
  std::array<float, 4 * 2 * 1> valid = { 1, 1, 2, 2, 1, 1, 2, 2 };

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());


  auto gpu_output = cle::Array::create(4, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier2::concatenate_along_x_func(device, gpu_input1, gpu_input2, gpu_output);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 4);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestConcatenate, alongX)
{
  std::array<float, 4 * 2 * 1> valid = { 1, 1, 2, 2, 1, 1, 2, 2 };

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier2::concatenate_along_x_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 4);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestConcatenate, alongY)
{
  std::array<float, 2 * 2 * 2> valid = { 1, 1, 1, 1, 2, 2, 2, 2 };

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier2::concatenate_along_y_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 4);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestConcatenate, alongZ)
{
  std::array<float, 2 * 2 * 2> valid = { 1, 1, 1, 1, 2, 2, 2, 2 };

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier2::concatenate_along_z_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 2);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestConcatenate, ::testing::ValuesIn(getParameters()));
