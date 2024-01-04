#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include <numeric>

class TestSumAllPixel : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 20 * 30> input;
  float                           valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), 1.0f);
    valid = std::accumulate(input.begin(), input.end(), 0.0f);
  }
};

TEST_P(TestSumAllPixel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->write(input.data());

  auto output = cle::tier2::sum_of_all_pixels_func(device, array);

  EXPECT_EQ(output, 10 * 20 * 30);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSumAllPixel, ::testing::ValuesIn(getParameters()));