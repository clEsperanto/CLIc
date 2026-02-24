#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestGradient : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 3> output;
  std::array<float, 3 * 3 * 3> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<float, 3 * 3 * 3> valid_x = { 0, 0, 0, 0, 0, 0,  0, 0, 0,

                                           0, 0, 0, 1, 0, -1, 0, 0, 0,

                                           0, 0, 0, 0, 0, 0,  0, 0, 0 };
  std::array<float, 3 * 3 * 3> valid_y = { 0, 0, 0, 0, 0, 0, 0, 0,  0,

                                           0, 1, 0, 0, 0, 0, 0, -1, 0,

                                           0, 0, 0, 0, 0, 0, 0, 0,  0 };
  std::array<float, 3 * 3 * 3> valid_z = { 0, 0, 0, 0, 1,  0, 0, 0, 0,

                                           0, 0, 0, 0, 0,  0, 0, 0, 0,

                                           0, 0, 0, 0, -1, 0, 0, 0, 0 };
};

TEST_P(TestGradient, executeX)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::gradient_x_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_x[i]);
  }
}

TEST_P(TestGradient, executeY)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::gradient_y_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_y[i]);
  }
}

TEST_P(TestGradient, executeZ)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::gradient_z_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_z[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestGradient, ::testing::ValuesIn(getParameters()));
