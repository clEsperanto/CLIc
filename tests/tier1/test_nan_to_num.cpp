#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestNanToNum : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 1 * 1> output;
  std::array<float, 5 * 1 * 1> input = { -std::numeric_limits<float>::infinity(),
                                         std::numeric_limits<float>::infinity(),
                                         std::numeric_limits<float>::quiet_NaN(),
                                         0,
                                         1 };
  std::array<float, 5 * 1 * 1> valid = { 5, 4, 3, 0, 1 };
};

TEST_P(TestNanToNum, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 1, 1,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::nan_to_num_func(device, gpu_input, nullptr, 3, 4, 5);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestNanToNum, ::testing::ValuesIn(getParameters()));
