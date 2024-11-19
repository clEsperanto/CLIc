#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include <numeric>

class TestCopySlice : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestCopySlice, executeCopySliceFrom)
{
  std::array<float, 2 * 2 * 1> output;
  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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

TEST_P(TestCopySlice, executeCopySliceTo)
{
  std::array<float, 2 * 2 * 2> output;
  std::array<float, 2 * 2 * 1> input = { 3, 4, 4, 5 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 5.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.0);
}

TEST_P(TestCopySlice, executeCopySliceToWithOneSlice)
{
  std::array<float, 3 * 2 * 1> output;
  std::array<float, 3 * 2 * 1> input = { 3, 4, 6, 4, 5, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(3, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 2.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 6.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 4.0);
}

TEST_P(TestCopySlice, executeCopySliceMinX)
{
  std::array<float, 4 * 1 * 1> output;
  std::array<float, 4 * 1 * 1> input = { 1, 2, 3, 4 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(4, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 1.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 4.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.5);
}

TEST_P(TestCopySlice, executeCopySliceMinY)
{
  std::array<float, 1 * 4 * 1> output;
  std::array<float, 1 * 4 * 1> input = { 1, 2, 3, 4 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(1, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(1, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 1.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 4.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.5);
}

std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCopySlice, ::testing::ValuesIn(getParameters()));
