#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestDifferenceOfGaussian : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<float, 3 * 3 * 3> output;
  std::array<float, 3 * 3 * 3> input;
  std::array<float, 3 * 3 * 3> valid = {
    static_cast<float>(1.217670321), static_cast<float>(2.125371218), static_cast<float>(1.217670321), static_cast<float>(2.125371456),
    static_cast<float>(3.62864542),  static_cast<float>(2.125371456), static_cast<float>(1.217670321), static_cast<float>(2.125371218),
    static_cast<float>(1.217670321), static_cast<float>(2.125371456), static_cast<float>(3.62864542),  static_cast<float>(2.125371456),
    static_cast<float>(3.628645658), static_cast<float>(6.114237785), static_cast<float>(3.628645658), static_cast<float>(2.125371456),
    static_cast<float>(3.62864542),  static_cast<float>(2.125371456), static_cast<float>(1.217670321), static_cast<float>(2.125371218),
    static_cast<float>(1.217670321), static_cast<float>(2.125371456), static_cast<float>(3.62864542),  static_cast<float>(2.125371456),
    static_cast<float>(1.217670321), static_cast<float>(2.125371218), static_cast<float>(1.217670321)
  };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
    std::fill(input.begin(), input.end(), 0.0f);
    const int center = (3 / 2) + (3 / 2) * 3 + (3 / 2) * 3 * 3;
    input[center] = 100.0f;
  }
};

TEST_P(TestDifferenceOfGaussian, execute)
{
  auto gpu_input = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::difference_of_gaussian_func(device, gpu_input, nullptr, 1, 1, 1, 3, 3, 3);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.0001);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDifferenceOfGaussian, ::testing::ValuesIn(getParameters()));
