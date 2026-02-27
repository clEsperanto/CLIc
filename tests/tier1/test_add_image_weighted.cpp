#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestAddImagesWeighted : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  const float value_1 = 25;
  const float value_2 = 75;
  const float factor1 = 0.5;
  const float factor2 = 0.25;

  virtual void
  SetUp()
  {
    std::fill(input1.begin(), input1.end(), value_1);
    std::fill(input2.begin(), input2.end(), value_2);
    std::fill(valid.begin(), valid.end(), static_cast<float>(value_1 * factor1 + value_2 * factor2));
  }
};

TEST_P(TestAddImagesWeighted, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());
  auto gpu_output = cle::tier1::add_images_weighted_func(device, gpu_input1, gpu_input2, nullptr, factor1, factor2);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestAddImagesWeighted, ::testing::ValuesIn(getParameters()));
