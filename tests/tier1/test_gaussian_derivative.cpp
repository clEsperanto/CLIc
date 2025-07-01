#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestGaussianDerivative : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int32_t, 5 * 5 * 1> input;

  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> valid = { 0.2915041745F, 1.306431174F, 2.153940439F, 1.306431174F, 0.2915041745F,
                                         1.306431055F,  5.855018139F, 9.653291702F, 5.855018139F, 1.306431055F,
                                         2.153940678F,  9.653292656F, 15.91558743F, 9.653292656F, 2.153940678F,
                                         1.306431055F,  5.855018139F, 9.653291702F, 5.855018139F, 1.306431055F,
                                         0.2915041745F, 1.306431174F, 2.153940439F, 1.306431174F, 0.2915041745F };

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), 0);
    const int center = (5 / 2) + (5 / 2) * 5;
    input[center] = 100;
  }
};

TEST_P(TestGaussianDerivative, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::gaussian_derivative_func(device, gpu_input, nullptr, 1, 1, 1, 1, 0, 0);

  cle::print<float>(gpu_output, "output");

  // gpu_output->readTo(output.data());
  // for (int i = 0; i < output.size(); i++)
  // {
  //   EXPECT_NEAR(output[i], valid[i], 0.0001);
  // }
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestGaussianDerivative, ::testing::ValuesIn(getParameters()));
