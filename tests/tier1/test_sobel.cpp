#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestSobel : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid = { 0, 0, 0, 0,       0, 0,       1.41421, 2, 1.41421, 0, 0, 2, 0,
                                         2, 0, 0, 1.41421, 2, 1.41421, 0,       0, 0,       0, 0, 0 };
};
TEST_P(TestSobel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::sobel_func(device, gpu_input, nullptr);

  gpu_output->read(output.data());

  for (auto && i : output)
  {
    std::cout << i << " ";
  }
  std::cout << std::endl;


  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.0001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSobel, ::testing::ValuesIn(getParameters()));
