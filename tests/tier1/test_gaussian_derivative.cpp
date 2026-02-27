#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestGaussianDerivative : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> input = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0,
                                         0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> valid = { 0.00518165, 0.0116113, 0, -0.0116113, -0.00518165, 0.0232225, 0.0520381, 0, -0.0520381, -0.0232225,
                                         0.0382875,  0.0857963, 0, -0.0857963, -0.0382875,  0.0232225, 0.0520381, 0, -0.0520381, -0.0232225,
                                         0.00518165, 0.0116113, 0, -0.0116113, -0.00518165 };
};

TEST_P(TestGaussianDerivative, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::gaussian_derivative_func(device, gpu_input, nullptr, 1.0, 1.0, 1.0, 1, 0, 0);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.0001);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestGaussianDerivative, ::testing::ValuesIn(getParameters()));
