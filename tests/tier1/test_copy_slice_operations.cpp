#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>
#include <numeric>

// Consolidated copy and slice operations tests
// Tests: COPY, COPY_SLICE, COPY_HORIZONTAL_SLICE, COPY_VERTICAL_SLICE

class TestCopySliceOperations : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

// 0 = COPY
TEST_P(TestCopySliceOperations, copy)
{
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(10));
  std::fill(valid.begin(), valid.end(), static_cast<float>(10));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::copy_func(device, gpu_input, nullptr);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = COPY_SLICE
TEST_P(TestCopySliceOperations, copy_slice)
{
  std::array<float, 2 * 2 * 1> output;
  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };

  auto gpu_input = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 4.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.25);
}

// 2 = COPY_HORIZONTAL_SLICE
TEST_P(TestCopySliceOperations, copy_horizontal_slice)
{
  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };
  std::array<float, 2 * 2 * 1> output;

  auto gpu_input = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_horizontal_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 1.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 4.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.25);
}

// 3 = COPY_VERTICAL_SLICE
TEST_P(TestCopySliceOperations, copy_vertical_slice)
{
  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };
  std::array<float, 2 * 2 * 1> output;

  auto gpu_input = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_vertical_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 1.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 0.75);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCopySliceOperations, ::testing::ValuesIn(getParameters()));
