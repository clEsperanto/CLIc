#include "cle.hpp"

#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated unary math operations tests
// Tests: ABSOLUTE, CUBIC_ROOT, SQUARE_ROOT, SIGN

class TestUnaryMathOperations : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

// 0 = ABSOLUTE
TEST_P(TestUnaryMathOperations, absolute)
{
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(-7));
  std::fill(valid.begin(), valid.end(), static_cast<float>(7));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::absolute_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = CUBIC_ROOT
TEST_P(TestUnaryMathOperations, cubic_root)
{
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(27));
  std::fill(valid.begin(), valid.end(), static_cast<float>(3));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::cubic_root_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01);
  }
}

// 2 = SQUARE_ROOT
TEST_P(TestUnaryMathOperations, square_root)
{
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(16));
  std::fill(valid.begin(), valid.end(), static_cast<float>(4));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::square_root_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01);
  }
}

// 3 = SIGN
TEST_P(TestUnaryMathOperations, sign)
{
  std::array<float, 30> output;
  std::array<float, 30> input = { -5, -1, 0, 1, 5, -2, -10, 0, 10, 2, -7, -3, 0, 3, 7, -6, -11, 0, 11, 6, -8, -4, 0, 4, 8, -9, -12, 0, 12, 9 };
  std::array<float, 30> valid = { -1, -1, 0, 1, 1, -1, -1, 0, 1, 1, -1, -1, 0, 1, 1, -1, -1, 0, 1, 1, -1, -1, 0, 1, 1, -1, -1, 0, 1, 1 };

  auto gpu_input = cle::Array::create(10, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::sign_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}


INSTANTIATE_TEST_SUITE_P(InstantiationName, TestUnaryMathOperations, ::testing::ValuesIn(getParameters()));
