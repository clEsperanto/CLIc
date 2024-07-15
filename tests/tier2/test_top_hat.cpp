#include "cle.hpp"

#include <algorithm>
#include <array>
#include <gtest/gtest.h>

class TestTopHat : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input = { 0,  0, 0, 0,  0,  0,  50, 50, 50, 0, 0, 50, 100,
                                         50, 0, 0, 50, 50, 50, 0,  0,  0,  0, 0, 0 };
};

TEST_P(TestTopHat, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::top_hat_box_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 50);
}

TEST_P(TestTopHat, executeSphere)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::top_hat_sphere_func(device, gpu_input, nullptr, 1, 1, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 50);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestTopHat, ::testing::ValuesIn(getParameters()));
