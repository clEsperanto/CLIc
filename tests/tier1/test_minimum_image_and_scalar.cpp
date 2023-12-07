#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMaximumImageAndScalar : public ::testing::TestWithParam<std::string>
{
protected:
  const float                   value = 5;
  const float                   scalar = 10;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(value));
    std::fill(valid.begin(), valid.end(), static_cast<float>(value));
    for (int i = 0; i < input.size(); i++)
    {
      if (i % 2 == 0)
      {
        input[i] = 42;
        valid[i] = scalar;
      }
    }
  }
};

TEST_P(TestMaximumImageAndScalar, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::minimum_image_and_scalar_func(device, gpu_input, nullptr, scalar);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMaximumImageAndScalar, ::testing::ValuesIn(getParameters()));
