#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestPadding : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 1> input = {
    1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
  };
};

TEST_P(TestPadding, smoothSize)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  auto smooth_size = cle::fft::get_next_smooth({ 10, 5, 1 });
  EXPECT_EQ(smooth_size[0], 10);
  EXPECT_EQ(smooth_size[1], 5);
  EXPECT_EQ(smooth_size[2], 1);

  smooth_size = cle::fft::get_next_smooth({ 11, 4, 7 });
  EXPECT_EQ(smooth_size[0], 12);
  EXPECT_EQ(smooth_size[1], 4);
  EXPECT_EQ(smooth_size[2], 7);

  smooth_size = cle::fft::get_next_smooth({ 25, 30, 2 });
  EXPECT_EQ(smooth_size[0], 25);
  EXPECT_EQ(smooth_size[1], 30);
  EXPECT_EQ(smooth_size[2], 2);

  smooth_size = cle::fft::get_next_smooth({ 10, 10, 11 });
  EXPECT_EQ(smooth_size[0], 10);
  EXPECT_EQ(smooth_size[1], 10);
  EXPECT_EQ(smooth_size[2], 12);

  smooth_size = cle::fft::get_next_smooth({ 10, 10, 10 });
  EXPECT_EQ(smooth_size[0], 10);
  EXPECT_EQ(smooth_size[1], 10);
  EXPECT_EQ(smooth_size[2], 10);
}

TEST_P(TestPadding, executePadding)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  std::array<float, 3 * 3 * 1> input = {
    0, 0, 0, 0, 1, 0, 0, 0, 0,
  };

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_pad = cle::tier1::pad_func(device, gpu_input, nullptr, 2, 3, 0, 5, false);
  auto gpu_output = cle::tier1::unpad_func(device, gpu_pad, nullptr, 2, 3, 0, false);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]);
  }
}

TEST_P(TestPadding, executePaddingCenter)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  cle::use_cache(false);

  std::array<float, 3 * 3 * 1> input = {
    0, 0, 0, 0, 1, 0, 0, 0, 0,
  };

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_pad = cle::tier1::pad_func(device, gpu_input, nullptr, 2, 3, 0, 5, true);
  auto gpu_output = cle::tier1::unpad_func(device, gpu_pad, nullptr, 2, 3, 0, true);
  
  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  EXPECT_EQ(output.size(), input.size());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], input[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestPadding, ::testing::ValuesIn(getParameters()));
