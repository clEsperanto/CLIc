#include "cle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>

class TestStandardDeviation : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid_box = { 0,     0, 0, 0,     0,     0,     0.314, 0.314, 0.314, 0, 0, 0.314, 0.314,
                                             0.314, 0, 0, 0.314, 0.314, 0.314, 0,     0,     0,     0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid_sphere = { 0,   0, 0, 0, 0,   0, 0, 0.4, 0, 0, 0, 0.4, 0.4,
                                                0.4, 0, 0, 0, 0.4, 0, 0, 0,   0, 0, 0, 0 };
};

TEST_P(TestStandardDeviation, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier2::standard_deviation_box_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_box[i], 0.001);
  }
}

TEST_P(TestStandardDeviation, executeSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier2::standard_deviation_sphere_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid_sphere[i], 0.0001);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStandardDeviation, ::testing::ValuesIn(getParameters()));
