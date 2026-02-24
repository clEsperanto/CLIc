#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestUndefinedToZero : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1> output;
  std::array<float, 5 * 5 * 1> input1 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> input2 = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestUndefinedToZero, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_intput1 = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_intput2 = cle::Array::create(gpu_intput1);
  gpu_intput1->writeFrom(input1.data());
  gpu_intput2->writeFrom(input2.data());

  auto gpu_undef = cle::tier1::divide_images_func(device, gpu_intput1, gpu_intput2, nullptr);
  auto gpu_output = cle::tier1::undefined_to_zero_func(device, gpu_undef, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestUndefinedToZero, ::testing::ValuesIn(getParameters()));
