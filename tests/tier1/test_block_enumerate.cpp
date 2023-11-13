#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestBlockEnumerate : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 1 * 1> output;
  std::array<float, 5 * 1 * 1> input;
  std::array<float, 5 * 1 * 1> valid;

  virtual void
  SetUp()
  {
    float index = 0;
    for (size_t i = 0; i < valid.size(); i++)
    {
      if (i % 2 == 0)
      {
        valid[i] = ++index;
        input[i] = 1;
      }
      else
      {
        valid[i] = 0;
        input[i] = 0;
      }
    }
  }
};

TEST_P(TestBlockEnumerate, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  int  blocksize = 4;
  auto gpu_input = cle::Array::create(5, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  size_t block_value = static_cast<size_t>((static_cast<size_t>(5 - 1) + 1) / blocksize) + 1;
  auto   gpu_temp = cle::Array::create(block_value, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);

  cle::tier1::sum_reduction_x_func(device, gpu_input, gpu_temp, blocksize);
  auto gpu_output = cle::tier1::block_enumerate_func(device, gpu_input, gpu_temp, nullptr, blocksize);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBlockEnumerate, ::testing::ValuesIn(getParameters()));
