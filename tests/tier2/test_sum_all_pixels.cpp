#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"
#include <numeric>

class TestSumAllPixel : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<float, 10 * 20 * 30> input;
  float                           valid;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
    std::fill(input.begin(), input.end(), 1.0f);
    valid = std::accumulate(input.begin(), input.end(), 0.0f);
  }
};

TEST_P(TestSumAllPixel, execute)
{
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->writeFrom(input.data());

  auto output = cle::tier2::sum_of_all_pixels_func(device, array);

  EXPECT_EQ(output, 10 * 20 * 30);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSumAllPixel, ::testing::ValuesIn(getParameters()));
