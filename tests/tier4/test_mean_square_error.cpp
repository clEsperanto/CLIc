
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMeanSquareError : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<float, 3 * 1 * 1> input1 = { 1, 2, 3 };
  const std::array<float, 3 * 1 * 1> input2 = { 4, 5, 7 };
};

TEST_P(TestMeanSquareError, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(3, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto output = cle::tier4::mean_squared_error_func(device, gpu_input1, gpu_input2);
  EXPECT_NEAR(output, 11.333, 0.001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMeanSquareError, ::testing::ValuesIn(getParameters()));