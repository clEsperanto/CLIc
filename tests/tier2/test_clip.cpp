#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestAbsoluteDifference : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 2 * 2 * 1> output;
  std::array<float, 2 * 2 * 1> input = { 0, 1, 2, 3 };
};

TEST_P(TestAbsoluteDifference, executeMinMax)
{
  std::array<float, 2 * 2 * 1> valid = { 1, 1, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(2, 2, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier2::clip_func(device, gpu_input, nullptr, 1, 2);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestAbsoluteDifference, ::testing::ValuesIn(getParameters()));
