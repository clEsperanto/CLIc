#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestPaste : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 4 * 1> output;
  std::array<float, 4 * 4 * 1> input1 = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1 };
  std::array<float, 2 * 1 * 1> input2 = { 1, 2 };
  std::array<float, 4 * 4 * 1> valid = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 1, 2, 1, 1, 1, 1, 1 };
};

TEST_P(TestPaste, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(4, 4, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(2, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  cle::tier1::paste_func(device, gpu_input2, gpu_input1, 1, 2, 0);

  gpu_input1->readTo(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestPaste, ::testing::ValuesIn(getParameters()));
