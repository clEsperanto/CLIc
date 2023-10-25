#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestConvolve : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 4 * 1> output;
  std::array<float, 3 * 3 * 1> kernel = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
  std::array<float, 4 * 4 * 1> input = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1 };
  std::array<float, 4 * 4 * 1> valid = { 3, 1, 0, 0, 1, 4, 0, 0, 0, 0, 4, 1, 0, 0, 1, 4 };
};

TEST_P(TestConvolve, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(4, 4, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());
  auto gpu_kernel = cle::Array::create(3, 3, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_kernel->write(kernel.data());

  auto gpu_output = cle::tier1::convolve_func(device, gpu_input, gpu_kernel, nullptr);

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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestConvolve, ::testing::ValuesIn(getParameters()));
