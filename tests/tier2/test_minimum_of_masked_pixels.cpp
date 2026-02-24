
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMinMaskPixel : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 3 * 2> input;
  std::array<float, 5 * 3 * 2> mask;
  float                        valid = 3;

  virtual void
  SetUp()
  {
    for (size_t i = 0; i < input.size(); i++)
    {
      input[i] = 100;
      mask[i] = (i % 2 == 0) ? 1 : 0;
    }
    input[2] = 3;
  }
};

TEST_P(TestMinMaskPixel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_mask = cle::Array::create(gpu_input);
  gpu_input->writeFrom(input.data());
  gpu_mask->writeFrom(mask.data());

  auto output = cle::tier2::minimum_of_masked_pixels_func(device, gpu_input, gpu_mask);

  EXPECT_EQ(output, valid);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMinMaskPixel, ::testing::ValuesIn(getParameters()));
