#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLaplaceBox : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 3 * 2> output;
  std::array<float, 4 * 3 * 2> input = { 1, 0, 2, 1, 0, 1, 0, 3, 0, 2, 0, 1, 3, 0, 0, 1, 0, 1, 1, 0, 2, 0, 2, 1 };
  std::array<float, 4 * 3 * 2> valid = { 4,  -22, 30,  -6, -21, 6, -26, 50,  -19, 34,  -28, -2,
                                         50, -23, -18, 3,  -27, 3, 5,   -26, 31,  -25, 28,  -1 };
};

TEST_P(TestLaplaceBox, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(4, 3, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::laplace_box_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestLaplaceBox, ::testing::ValuesIn(getParameters()));
