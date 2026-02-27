#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestMultiplyPixelAndCoord : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 3 * 1> output;
  std::array<float, 5 * 3 * 1> input = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2 };
  std::array<float, 5 * 3 * 1> valid = { 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 2, 4, 6, 8 };
};

TEST_P(TestMultiplyPixelAndCoord, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::multiply_image_and_position_func(device, gpu_input, nullptr, 0);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestMultiplyPixelAndCoord, returnType)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  for (cle::dType type : { cle::dType::UINT8,
                           cle::dType::INT8,
                           cle::dType::UINT16,
                           cle::dType::INT16,
                           cle::dType::FLOAT,
                           cle::dType::UINT32,
                           cle::dType::INT32 })
  {
    auto gpu_input = cle::Array::create(5, 3, 1, 3, type, cle::mType::BUFFER, device);
    auto gpu_output = cle::tier1::multiply_image_and_position_func(device, gpu_input, nullptr, 0);
    EXPECT_EQ(gpu_output->dtype(), cle::dType::FLOAT);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMultiplyPixelAndCoord, ::testing::ValuesIn(getParameters()));
