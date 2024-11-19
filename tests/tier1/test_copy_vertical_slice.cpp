#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include <numeric>

class TestCopyVerticalSlice : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestCopyVerticalSlice, executeFrom)
{
  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };
  std::array<float, 2 * 2 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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


TEST_P(TestCopyVerticalSlice, executeTo)
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

  cle::tier1::copy_vertical_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 5.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.0);
}

TEST_P(TestCopyVerticalSlice, executeIMG)
{
  std::array<float, 3 * 3 * 3> input = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  10, 11, 12, 13, 14,
                                         15, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28 };
  std::array<float, 3 * 3 * 1> valid = { 1, 4, 7, 11, 14, 17, 21, 24, 27 };
  std::array<float, 3 * 3 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_vertical_slice_func(device, gpu_input, gpu_output, 1);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCopyVerticalSlice, ::testing::ValuesIn(getParameters()));
