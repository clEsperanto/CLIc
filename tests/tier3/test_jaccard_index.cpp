#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestJaccardIndex : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestJaccardIndex, execute2D)
{
  std::array<float, 5 * 2 * 1> input1 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 };
  std::array<float, 5 * 2 * 1> input2 = { 0, 1, 1, 0, 0, 0, 1, 1, 0, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(5, 2, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto output = cle::tier3::jaccard_index_func(device, gpu_input1, gpu_input2);

  EXPECT_EQ(output, 0.5);
}

TEST_P(TestJaccardIndex, execute3D)
{
  std::array<float, 3 * 2 * 2> input1 = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0 };
  std::array<float, 3 * 2 * 2> input2 = { 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(3, 2, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->write(input1.data());
  gpu_input2->write(input2.data());

  auto output = cle::tier3::jaccard_index_func(device, gpu_input1, gpu_input2);

  EXPECT_EQ(output, 0.5);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestJaccardIndex, ::testing::ValuesIn(getParameters()));
