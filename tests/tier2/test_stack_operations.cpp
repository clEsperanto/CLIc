#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestStackOperations : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                  backend;
  cle::Device::Pointer         device;
  std::array<float, 2 * 2 * 6> input;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);

    // each slice is filled with its z index
    for (int i = 0; i < input.size(); i++)
    {
      input[i] = static_cast<float>(i / 4);
    }
  }
};

TEST_P(TestStackOperations, subStack)
{
  std::array<float, 2 * 2 * 3> output;
  std::array<float, 2 * 2 * 3> valid = { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };

  auto gpu_input = cle::Array::create(2, 2, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::sub_stack_func(device, gpu_input, nullptr, 1, 3);

  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 3);
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestStackOperations, subStackInvertedRange)
{
  std::array<float, 2 * 2 * 1> output;
  std::array<float, 2 * 2 * 1> valid = { 4, 4, 4, 4 };

  auto gpu_input = cle::Array::create(2, 2, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  // end_z < start_z is clamped to a single slice
  auto gpu_output = cle::tier2::sub_stack_func(device, gpu_input, nullptr, 4, 2);

  EXPECT_EQ(gpu_output->depth(), 1);
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestStackOperations, reduceStack)
{
  std::array<float, 2 * 2 * 3> output;
  std::array<float, 2 * 2 * 3> valid = { 0, 0, 0, 0, 2, 2, 2, 2, 4, 4, 4, 4 };

  auto gpu_input = cle::Array::create(2, 2, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::reduce_stack_func(device, gpu_input, nullptr, 2, 0);

  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 3);
  EXPECT_EQ(gpu_output->dtype(), cle::dType::FLOAT);
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestStackOperations, reduceStackWithOffset)
{
  std::array<float, 2 * 2 * 2> output;
  std::array<float, 2 * 2 * 2> valid = { 1, 1, 1, 1, 4, 4, 4, 4 };

  auto gpu_input = cle::Array::create(2, 2, 6, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier2::reduce_stack_func(device, gpu_input, gpu_output, 3, 1);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStackOperations, ::testing::ValuesIn(getParameters()));
