#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestRamp : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int32_t, 3 * 3 * 2> output;
  std::array<int32_t, 3 * 3 * 2> input;
  std::array<int32_t, 3 * 3 * 2> valid_x = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 2 };
  std::array<int32_t, 3 * 3 * 2> valid_y = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2 };
  std::array<int32_t, 3 * 3 * 2> valid_z = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

  virtual void
  SetUp()
  {
    input = { 0, 0, 0, 3, 4, 3, 3, 4, 3, 3, 4, 3, 3, 4, 3, 3, 4, 3 };
  }
};


TEST_P(TestRamp, executeRampX)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 2,3 , cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_ramp_x_func(device, gpu_input);

  gpu_input->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_x[i]);
  }
}

TEST_P(TestRamp, executeRampY)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 2,3 , cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_ramp_y_func(device, gpu_input);

  gpu_input->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_y[i]);
  }
}

TEST_P(TestRamp, executeRampZ)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 2,3 , cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_ramp_z_func(device, gpu_input);

  gpu_input->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_z[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestRamp, ::testing::ValuesIn(getParameters()));
