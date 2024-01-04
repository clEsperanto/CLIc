#include "cle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>

class TestRadDeg : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 1 * 1> output;
  std::array<float, 4 * 1 * 1> input_rad = { M_PI, 0, 0.785398, -M_PI * 0.5 };
  std::array<float, 4 * 1 * 1> input_deg = { 180, 0, 45, -90 };
};

TEST_P(TestRadDeg, radToDeg)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input_deg.data());

  auto gpu_output = cle::tier2::degrees_to_radians_func(device, gpu_input, nullptr);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], input_rad[i], 0.001);
  }
}

TEST_P(TestRadDeg, degToRad)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input_rad.data());

  auto gpu_output = cle::tier2::radians_to_degrees_func(device, gpu_input, nullptr);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], input_deg[i], 0.001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestRadDeg, ::testing::ValuesIn(getParameters()));
