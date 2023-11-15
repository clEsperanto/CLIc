#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestUndefinedToZero : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input1 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> input2 = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestUndefinedToZero, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_intput1 = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_intput2 = cle::Array::create(gpu_intput1);
  gpu_intput1->write(input1.data());
  gpu_intput2->write(input2.data());

  auto gpu_undef = cle::tier1::divide_images_func(device, gpu_intput1, gpu_intput2, nullptr);
  auto gpu_output = cle::tier1::undefined_to_zero_func(device, gpu_undef, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestUndefinedToZero, ::testing::ValuesIn(getParameters()));
