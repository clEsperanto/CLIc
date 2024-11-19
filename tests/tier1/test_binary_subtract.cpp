#include "cle.hpp"

#include <array>
#include <random>

#include <gtest/gtest.h>

class TestBinarySubtract : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input1;
  std::array<uint8_t, 10 * 5 * 3> input2;
  std::array<uint8_t, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    static std::uniform_int_distribution<int> distribution(0, 1);
    static std::default_random_engine         generator;
    std::generate(input1.begin(), input1.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
    std::generate(input2.begin(), input2.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
    for (size_t i = 0; i < valid.size(); i++)
    {
      valid[i] = ((input1[i] - input2[i]) <= 0) ? 0 : input1[i];
    }
  }
};

TEST_P(TestBinarySubtract, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::binary_subtract_func(device, gpu_input1, gpu_input2, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBinarySubtract, ::testing::ValuesIn(getParameters()));
