#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestCombineLabels : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 2 * 1> input1 = { 1, 1, 0, 0, 3, 3, 1, 1, 0, 2, 3, 3 };
  std::array<uint32_t, 6 * 2 * 1> input2 = { 0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2, 0 };
  std::array<uint32_t, 6 * 2 * 1> valid = { 1, 3, 3, 4, 4, 2, 1, 3, 3, 4, 4, 2 };
  std::array<uint32_t, 6 * 2 * 1> output;
};

TEST_P(TestCombineLabels, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(6, 2, 1, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier5::combine_labels_func(device, gpu_input1, gpu_input2, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCombineLabels, ::testing::ValuesIn(getParameters()));
