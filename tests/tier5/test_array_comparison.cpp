#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestArrayComparisons : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int32_t, 3 * 1 * 1> input1 = { 1, 2, 3 };
  std::array<int16_t, 3 * 1 * 1> input2 = { 4, 5, 6 };
  std::array<int8_t, 4 * 1 * 1>  input3 = { 1, 2, 3, 3 };
  std::array<float, 3 * 1 * 1>   input4 = { 1.0F, 2.0F, 3.0F };
};

TEST_P(TestArrayComparisons, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(3, 1, 1, 3, cle::dType::INT32, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(3, 1, 1, 3, cle::dType::INT16, cle::mType::BUFFER, device);
  auto gpu_input3 = cle::Array::create(4, 1, 1, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input4 = cle::Array::create(3, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input5 = cle::Array::create(1, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());
  gpu_input3->writeFrom(input3.data());
  gpu_input4->writeFrom(input4.data());
  gpu_input5->writeFrom(input4.data());

  EXPECT_FALSE(cle::tier5::array_equal_func(device, gpu_input1, gpu_input2));
  EXPECT_FALSE(cle::tier5::array_equal_func(device, gpu_input1, gpu_input3));
  EXPECT_TRUE(cle::tier5::array_equal_func(device, gpu_input1, gpu_input4));
  EXPECT_FALSE(cle::tier5::array_equal_func(device, gpu_input1, gpu_input5));
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestArrayComparisons, ::testing::ValuesIn(getParameters()));
