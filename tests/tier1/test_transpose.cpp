#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestTranspose : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 4 * 3 * 2> output;
  std::array<float, 4 * 3 * 2> inputXYZ = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
  std::array<float, 3 * 4 * 2> inputYXZ = { 1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12, 13, 17, 21, 14, 18, 22, 15, 19, 23, 16, 20, 24 };
  std::array<float, 4 * 2 * 3> inputXZY = { 1, 2, 3, 4, 13, 14, 15, 16, 5, 6, 7, 8, 17, 18, 19, 20, 9, 10, 11, 12, 21, 22, 23, 24 };
  std::array<float, 2 * 3 * 4> inputZYX = { 1, 13, 5, 17, 9, 21, 2, 14, 6, 18, 10, 22, 3, 15, 7, 19, 11, 23, 4, 16, 8, 20, 12, 24 };
  std::array<float, 4 * 3 * 2> valid;
};

TEST_P(TestTranspose, executeXY)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(inputXYZ.data());

  auto gpu_output = cle::tier1::transpose_xy_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], inputYXZ[i]);
  }
  EXPECT_EQ(gpu_output->width(), 3);
  EXPECT_EQ(gpu_output->height(), 4);
  EXPECT_EQ(gpu_output->depth(), 2);
}

TEST_P(TestTranspose, executeXZ)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(inputXYZ.data());

  auto gpu_output = cle::tier1::transpose_xz_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], inputZYX[i]);
  }
  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 3);
  EXPECT_EQ(gpu_output->depth(), 4);
}

TEST_P(TestTranspose, executeYZ)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(inputXYZ.data());

  auto gpu_output = cle::tier1::transpose_yz_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], inputXZY[i]);
  }
  EXPECT_EQ(gpu_output->width(), 4);
  EXPECT_EQ(gpu_output->height(), 2);
  EXPECT_EQ(gpu_output->depth(), 3);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestTranspose, ::testing::ValuesIn(getParameters()));
