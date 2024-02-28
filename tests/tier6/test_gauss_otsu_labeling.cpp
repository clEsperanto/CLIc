#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestGaussOtsuLabeling : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 7 * 1> valid = { 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 0 };

  std::array<uint32_t, 6 * 7 * 1> input = { 0, 0, 1, 1, 0, 0, 0, 1, 8, 9, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 8, 7, 1, 0, 0, 1, 1, 1, 0 };

  std::array<uint32_t, 6 * 7 * 1> output;
};

TEST_P(TestGaussOtsuLabeling, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 7, 1, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier6::gauss_otsu_labeling_func(device, gpu_input, nullptr, 1);

  gpu_output->read(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestGaussOtsuLabeling, ::testing::ValuesIn(getParameters()));