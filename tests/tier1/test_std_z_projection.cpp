#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestStdProjectionZ : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 5> input = { 1, 0, 0, 0, 9, 0, 2, 0, 8, 0,  3, 0, 1, 0, 10, 0, 4, 0, 7, 0,  5, 0, 6, 0, 10,
                                         0, 2, 0, 8, 0, 1, 0, 0, 0, 9,  3, 0, 1, 0, 10, 0, 4, 0, 7, 0,  5, 0, 6, 0, 10,
                                         0, 2, 0, 8, 0, 3, 0, 1, 0, 10, 0, 4, 0, 7, 0,  1, 0, 0, 0, 9,  5, 0, 6, 0, 10,
                                         0, 2, 0, 8, 0, 1, 0, 0, 0, 9,  0, 4, 0, 7, 0,  3, 0, 1, 0, 10, 5, 0, 6, 0, 10,
                                         1, 0, 0, 0, 9, 0, 4, 0, 7, 0,  3, 0, 1, 0, 10, 0, 2, 0, 8, 0,  5, 0, 6, 0, 10 };
  std::array<float, 5 * 5 * 1> valid = { 0.55, 1.10, 0,    4.38, 4.93, 1.22, 1.79, 0.45, 4.12, 5.13, 1.64, 2.19, 0.55,
                                         3.83, 5.48, 1.30, 2,    0.45, 4.03, 5.22, 0,    0,    0,    0,    0 };
};

TEST_P(TestStdProjectionZ, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::std_z_projection_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStdProjectionZ, ::testing::ValuesIn(getParameters()));
