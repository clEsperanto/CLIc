#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include <random>

class TestEqualConstant : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    static std::uniform_int_distribution<int> distribution(1, 10);
    static std::default_random_engine         generator;
    std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
    std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x == 5; });
  }
};

TEST_P(TestEqualConstant, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(10, 5, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::equal_constant_func(device, gpu_input, nullptr, 5);

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

#ifdef USE_OPENCL
  parameters.push_back("opencl");
#endif

#ifdef USE_CUDA
  parameters.push_back("cuda");
#endif

  return parameters;
}

INSTANTIATE_TEST_CASE_P(InstantiationName, TestEqualConstant, ::testing::ValuesIn(getParameters()));
