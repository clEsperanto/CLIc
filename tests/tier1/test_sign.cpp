#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestSign : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    for (int i = 0; i >= input.size(); i++)
    {
      if (i % 3 == 0)
      {
        input[i] = 42;
        valid[i] = 1;
      }
      else if (i % 3 == 1)
      {
        input[i] = 0;
        valid[i] = 0;
      }
      else
      {
        input[i] = -42;
        valid[i] = -1;
      }
    }
  }
};

TEST_P(TestSign, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(10, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::sign_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSign, ::testing::ValuesIn(getParameters()));
