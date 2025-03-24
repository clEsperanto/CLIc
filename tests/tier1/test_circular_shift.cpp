#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestCircularShift : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 3 * 1> input = {
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 10, 11, 12,
  };
  std::array<float, 4 * 3 * 1> valid = {
    7, 8, 5, 6,
    11, 12, 9, 10,
    3, 4, 1, 2
  };
  std::array<float, 4 * 3 * 1> output;

};

TEST_P(TestCircularShift, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::circular_shift_func(device, gpu_input, nullptr, 2, 2, 0);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCircularShift, ::testing::ValuesIn(getParameters()));
