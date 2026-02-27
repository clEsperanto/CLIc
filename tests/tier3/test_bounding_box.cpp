#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestBoundingBox : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> input2d = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<float, 5 * 5 * 2> input3d = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                           0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestBoundingBox, execute2d)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input2d.data());

  auto output = cle::tier3::bounding_box_func(device, gpu_input);

  std::vector<float> valid = { 1, 1, 0, 2, 2, 0 };
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestBoundingBox, execute3d)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 2, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input3d.data());

  auto output = cle::tier3::bounding_box_func(device, gpu_input);

  std::vector<float> valid = { 1, 1, 0, 2, 2, 1 };
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestBoundingBox, largePositionInt8)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<int8_t, 265 * 265 * 1> input_large = {};
  for (int i = 0; i < input_large.size(); i++)
  {
    input_large[i] = 0;
  }
  input_large[265 * 265 - 1] = 1;

  auto gpu_input = cle::Array::create(265, 265, 1, 2, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_large.data());

  auto output = cle::tier3::bounding_box_func(device, gpu_input);

  std::vector<float> valid = { 264, 264, 0, 264, 264, 0 };
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBoundingBox, ::testing::ValuesIn(getParameters()));
