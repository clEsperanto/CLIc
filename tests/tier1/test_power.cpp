#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestPower : public ::testing::TestWithParam<std::string>
{
protected:
  const float                   value = 5;
  const float                   exp = 3;
  std::string backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
  std::string backend = GetParam();
  cle::BackendManager::getInstance().setBackend(backend);
  device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);
  }
};

TEST_P(TestPower, execute_w_float)
{

  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(value));
  std::fill(valid.begin(), valid.end(), static_cast<float>(std::pow(value, exp)));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::power_func(device, gpu_input, nullptr, exp);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestPower, execute_w_int)
{

  std::array<int32_t, 10 * 5 * 3> output;
  std::array<int32_t, 10 * 5 * 3> input;
  std::array<int32_t, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<int32_t>(value));
  std::fill(valid.begin(), valid.end(), static_cast<int32_t>(std::pow(value, exp)));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::power_func(device, gpu_input, nullptr, exp);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestPower, ::testing::ValuesIn(getParameters()));
