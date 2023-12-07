#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestRange : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 6 * 6 * 1> input = { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3,
                                         4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6 };
};


TEST_P(TestRange, executeVertical)
{
  std::array<float, 6 * 3 * 1> output;
  std::array<float, 6 * 3 * 1> valid = { 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 1,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::range_func(device, gpu_input, nullptr, 0, 6, 1, 0, 6, 2, 1, 1, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestRange, executeDiag)
{
  std::array<float, 3 * 3 * 1> output;
  std::array<float, 3 * 3 * 1> valid = { 1, 1, 1, 3, 3, 3, 5, 5, 5 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 1,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::range_func(device, gpu_input, nullptr, 0, 6, 2, 0, 6, 2, 1, 1, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestRange, executeNegative)
{
  std::array<float, 6 * 3 * 1> output;
  std::array<float, 6 * 3 * 1> valid = { 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 1,3 , cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::range_func(device, gpu_input, nullptr, 0, 6, 1, 6, 0, -2, 1, 1, 1);

  gpu_output->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestRange, ::testing::ValuesIn(getParameters()));
