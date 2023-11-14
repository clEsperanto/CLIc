#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMinimumProjectionX : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 1 * 5 * 3>  output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 1 * 5 * 3>  valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(10));
    std::fill(valid.begin(), valid.end(), static_cast<float>(1));
    for (auto it = input.begin(); it != input.end(); std::advance(it, 10))
    {
      int idx = (it - input.begin()) + (rand() % 10);
      input[idx] = static_cast<float>(1);
    }
  }
};

TEST_P(TestMinimumProjectionX, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::minimum_x_projection_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMinimumProjectionX, ::testing::ValuesIn(getParameters()));
