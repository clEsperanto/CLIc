#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestMultiplyPixelAndCoord : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 3 * 1> output;
  std::array<float, 5 * 3 * 1> input = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
  std::array<float, 5 * 3 * 1> valid = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 2, 4, 6, 8 };
};

TEST_P(TestMultiplyPixelAndCoord, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 3, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::multiply_image_and_coordinate_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMultiplyPixelAndCoord, ::testing::ValuesIn(getParameters()));
