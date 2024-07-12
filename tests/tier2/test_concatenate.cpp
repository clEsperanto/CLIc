#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestConcatenate : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 2 * 2 * 2> output;
  std::array<float, 2 * 2 * 1> input1 = { 1, 1, 1, 1 };
  std::array<float, 2 * 2 * 1> input2 = { 2, 2, 2, 2 };
};

TEST_P(TestConcatenate, alongX)
{
  std::array<float, 4 * 2 * 1> valid = { 1, 1, 2, 2, 1, 1, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier2::concatenate_along_x_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 4);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestConcatenate, alongY)
{
  std::array<float, 2 * 2 * 2> valid = { 1, 1, 1, 1, 2, 2, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier2::concatenate_along_y_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 4);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestConcatenate, alongZ)
{
  std::array<float, 2 * 2 * 2> valid = { 1, 1, 1, 1, 2, 2, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier2::concatenate_along_z_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 2);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestConcatenate, ::testing::ValuesIn(getParameters()));
