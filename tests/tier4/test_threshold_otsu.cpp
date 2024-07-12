
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestThresholdOtsu : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 6.0058594 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };    // 6.00392 clic
  std::array<uint8_t, 3 * 2 * 2>       output;
};

TEST_P(TestThresholdOtsu, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestThresholdOtsu, ::testing::ValuesIn(getParameters()));
