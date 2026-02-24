#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMinimumImages : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    const float value1 = static_cast<float>(rand() % 4) + 1;
    const float value2 = static_cast<float>(rand() % 2) + 1;
    std::fill(input1.begin(), input1.end(), static_cast<float>(value1));
    std::fill(input2.begin(), input2.end(), static_cast<float>(value2));
    std::fill(valid.begin(), valid.end(), static_cast<float>(std::min(value1, value2)));
  }
};

TEST_P(TestMinimumImages, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::minimum_images_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMinimumImages, ::testing::ValuesIn(getParameters()));
