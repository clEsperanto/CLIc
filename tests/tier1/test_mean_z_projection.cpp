#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMeanProjectionZ : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 10 * 1> output;
  std::array<float, 10 * 10 * 2> input = {
    1,  2, 3,  4, 5,  6, 7, 8,  9, 10, 2, 3,  4, 5,  6, 7, 8,  9, 10, 1, 3,  4, 5,  6, 7, 8,  9, 10, 1, 2,  4, 5, 6, 7,
    8,  9, 10, 1, 2,  3, 5, 6,  7, 8,  9, 10, 1, 2,  3, 4, 6,  7, 8,  9, 10, 1, 2,  3, 4, 5,  7, 8,  9, 10, 1, 2, 3, 4,
    5,  6, 8,  9, 10, 1, 2, 3,  4, 5,  6, 7,  9, 10, 1, 2, 3,  4, 5,  6, 7,  8, 10, 1, 2, 3,  4, 5,  6, 7,  8, 9, 1, 2,
    3,  4, 5,  6, 7,  8, 9, 10, 2, 3,  4, 5,  6, 7,  8, 9, 10, 1, 3,  4, 5,  6, 7,  8, 9, 10, 1, 2,  4, 5,  6, 7, 8, 9,
    10, 1, 2,  3, 5,  6, 7, 8,  9, 10, 1, 2,  3, 4,  6, 7, 8,  9, 10, 1, 2,  3, 4,  5, 7, 8,  9, 10, 1, 2,  3, 4, 5, 6,
    8,  9, 10, 1, 2,  3, 4, 5,  6, 7,  9, 10, 1, 2,  3, 4, 5,  6, 7,  8, 10, 1, 2,  3, 4, 5,  6, 7,  8, 9,
  };
  std::array<float, 10 * 10 * 1> valid = { 1, 2,  3, 4,  5, 6,  7, 8,  9, 10, 2,  3, 4,  5, 6,  7, 8,  9, 10, 1,
                                           3, 4,  5, 6,  7, 8,  9, 10, 1, 2,  4,  5, 6,  7, 8,  9, 10, 1, 2,  3,
                                           5, 6,  7, 8,  9, 10, 1, 2,  3, 4,  6,  7, 8,  9, 10, 1, 2,  3, 4,  5,
                                           7, 8,  9, 10, 1, 2,  3, 4,  5, 6,  8,  9, 10, 1, 2,  3, 4,  5, 6,  7,
                                           9, 10, 1, 2,  3, 4,  5, 6,  7, 8,  10, 1, 2,  3, 4,  5, 6,  7, 8,  9 };
};

TEST_P(TestMeanProjectionZ, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(10, 10, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::mean_z_projection_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMeanProjectionZ, ::testing::ValuesIn(getParameters()));
