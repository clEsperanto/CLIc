#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMultiplyMatrix : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 4 * 1> output;
  std::array<float, 3 * 4 * 1> input1 = { 1, 0, 1, 2, 1, 1, 0, 1, 1, 1, 1, 2 };
  std::array<float, 3 * 3 * 1> input2 = { 1, 2, 1, 2, 3, 1, 4, 2, 2 };
  std::array<float, 3 * 4 * 1> valid = { 5, 4, 3, 8, 9, 5, 6, 5, 3, 11, 9, 6 };
};

TEST_P(TestMultiplyMatrix, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(3, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto gpu_output = cle::tier1::multiply_matrix_func(device, gpu_input1, gpu_input2, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMultiplyMatrix, ::testing::ValuesIn(getParameters()));
