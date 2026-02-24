#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestSubtractImageFromScalar : public ::testing::TestWithParam<std::string>
{
protected:
  const float                  value = 10;
  const float                  scalar = 5;
  std::array<float, 5 * 5 * 3> output;
  std::array<float, 5 * 5 * 3> input;
  std::array<float, 5 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(value));
    std::fill(valid.begin(), valid.end(), static_cast<float>(scalar - value));
  }
};

TEST_P(TestSubtractImageFromScalar, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::subtract_image_from_scalar_func(device, gpu_input, nullptr, scalar);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSubtractImageFromScalar, ::testing::ValuesIn(getParameters()));
