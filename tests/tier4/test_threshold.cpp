
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestThreshold : public ::testing::TestWithParam<std::string>
{
protected:
};

TEST_P(TestThreshold, otsu)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 6.0058594 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };    // 6.00392 clic
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestThreshold, otsuLowIntensity)
{
  const std::array<float, 3 * 3 * 1>   input_low = { 0, 0, 0, 0, 0.003, 0, 0, 0, 0 };
  const std::array<uint8_t, 3 * 3 * 1> valid_low = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
  std::array<uint8_t, 3 * 3 * 1>       output_low;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_low.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  gpu_output->readTo(output_low.data());
  for (int i = 0; i < output_low.size(); i++)
  {
    EXPECT_EQ(output_low[i], valid_low[i]);
  }
}

TEST_P(TestThreshold, yen)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 6.0058594 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };    // 6.00392 clic
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_yen_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestThreshold, ::testing::ValuesIn(getParameters()));
