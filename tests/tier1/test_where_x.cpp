#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestWhereX : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<int32_t, 4 * 4 * 1> output;
  std::array<int32_t, 4 * 4 * 1> input;
  std::array<int32_t, 4 * 4 * 1> valid_smaller = { 0, 0, 0, 1, 3, 0, 3, 1, 3, 3, 3, 1, 3, 3, 3, 1 };
  std::array<int32_t, 4 * 4 * 1> valid_equal = { 3, 0, 0, 1, 0, 3, 3, 1, 0, 0, 3, 1, 1, 1, 1, 3 };
  std::array<int32_t, 4 * 4 * 1> valid_greater = { 0, 3, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 1, 1, 1, 1 };

  virtual void
  SetUp()
  {
    input = { 0, 0, 0, 1, 0, 0, 3, 1, 0, 0, 3, 1, 1, 1, 1, 1 };
  }
};


TEST_P(TestWhereX, executeSmallerY)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 4, 1,3 , cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_where_x_smaller_than_y_func(device, gpu_input, 3);

  gpu_input->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_smaller[i]);
  }
}

TEST_P(TestWhereX, executeEqualY)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 4, 1,3 , cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_where_x_equals_y_func(device, gpu_input, 3);

  gpu_input->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_equal[i]);
  }
}

TEST_P(TestWhereX, executeGreaterY)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 4, 1,3 , cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  cle::tier1::set_where_x_greater_than_y_func(device, gpu_input, 3);

  gpu_input->read(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_greater[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestWhereX, ::testing::ValuesIn(getParameters()));
