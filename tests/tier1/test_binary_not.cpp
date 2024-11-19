#include "cle.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <random>


#include <gtest/gtest.h>

class TestBinaryNOT : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input;
  std::array<uint8_t, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    static std::uniform_int_distribution<int> distribution(0, 1);
    static std::default_random_engine         generator;
    std::generate(input.begin(), input.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
    std::transform(input.begin(), input.end(), valid.begin(), [](const uint8_t & x) {
      return static_cast<uint8_t>(std::abs(1 - x));
    });
  }
};

TEST_P(TestBinaryNOT, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::binary_not_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBinaryNOT, ::testing::ValuesIn(getParameters()));
