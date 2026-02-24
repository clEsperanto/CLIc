
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMinAllPixel : public ::testing::TestWithParam<std::string>
{
protected:
  const float                     max = 100;
  const float                     min = 42;
  std::array<float, 10 * 20 * 30> input;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), max);
    const int center = (10 / 2) + (20 / 2) * 10 + (30 / 2) * 10 * 20;
    input[center] = min;
  }
};

TEST_P(TestMinAllPixel, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->writeFrom(input.data());

  auto output = cle::tier2::minimum_of_all_pixels_func(device, array);

  EXPECT_EQ(output, min);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMinAllPixel, ::testing::ValuesIn(getParameters()));
