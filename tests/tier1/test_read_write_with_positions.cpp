#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestCoordReadWrite : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 1> input = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
};

TEST_P(TestCoordReadWrite, executeReadFromCoord1)
{
  std::array<float, 1 * 1 * 1> output;
  std::array<float, 1 * 2 * 1> list = { 1, 2 };
  std::array<float, 1 * 1 * 1> valid = { 8 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_list = cle::Array::create(1, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_list->writeFrom(list.data());

  auto gpu_output = cle::tier1::read_values_from_positions_func(device, gpu_input, gpu_list, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestCoordReadWrite, executeReadFromCoord2)
{
  std::array<float, 3 * 1 * 1> output;
  std::array<float, 3 * 2 * 1> list = { 0, 1, 0, 0, 2, 2 };
  std::array<float, 3 * 1 * 1> valid = { 1, 8, 7 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_list = cle::Array::create(3, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_list->writeFrom(list.data());

  auto gpu_output = cle::tier1::read_values_from_positions_func(device, gpu_input, gpu_list, nullptr);

  gpu_output->readTo(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCoordReadWrite, ::testing::ValuesIn(getParameters()));
