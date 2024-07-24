#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMaskLabel : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 0, 0, 3, 3, 4, 0, 0, 4, 4, 5, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> mask = { 0, 0, 0, 0, 0, 0, 2, 1, 3, 0, 0, 2, 1, 3, 0, 0, 1, 1, 3, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestMaskLabel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_mask = cle::Array::create(gpu_input);
  gpu_input->writeFrom(input.data());
  gpu_mask->writeFrom(mask.data());

  auto gpu_output = cle::tier1::mask_label_func(device, gpu_input, gpu_mask, nullptr, 1);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMaskLabel, ::testing::ValuesIn(getParameters()));
