
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMaxAllPixel : public ::testing::TestWithParam<std::string>
{
protected:
  const float                     max = 100;
  const float                     min = 42;
  std::array<float, 10 * 20 * 30> input;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), min);
    const int center = (10 / 2) + (20 / 2) * 10 + (30 / 2) * 10 * 20;
    input[center] = max;
  }
};

TEST_P(TestMaxAllPixel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto array = cle::Array::create(10, 20, 30, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->write(input.data());

  auto output = cle::tier2::maximum_of_all_pixels_func(device, array);

  EXPECT_EQ(output, max);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMaxAllPixel, ::testing::ValuesIn(getParameters()));