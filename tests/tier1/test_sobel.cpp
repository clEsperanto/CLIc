#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestSobel : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid = { 0,       0, 0,       0,       0,       0,       1.41421, 2,       1.41421,
                                         0,       0, 3.16228, 2,       3.16228, 0,       0,       3.16228, 2,
                                         3.16228, 0, 0,       1.41421, 2,       1.41421, 0 };
};


TEST_P(TestSobel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::sobel_func(device, gpu_input, nullptr);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.0001);
  }
}

std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;

#ifdef USE_OPENCL
  parameters.push_back("opencl");
#endif

#ifdef USE_CUDA
  parameters.push_back("cuda");
#endif

  return parameters;
}

INSTANTIATE_TEST_CASE_P(InstantiationName, TestSobel, ::testing::ValuesIn(getParameters()));
