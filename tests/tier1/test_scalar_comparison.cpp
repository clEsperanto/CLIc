#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>
#include <random>

// Consolidated scalar comparison operations tests
// Tests: EQUAL_CONSTANT, GREATER_CONSTANT, SMALLER_CONSTANT,
// GREATER_OR_EQUAL_CONSTANT, SMALLER_OR_EQUAL_CONSTANT, NOT_EQUAL_CONSTANT
// Compares image with a scalar constant

class TestScalarComparison : public ::testing::TestWithParam<std::string>
{
protected:
  static constexpr int8_t CONSTANT_VALUE = 5;
  std::string             backend;
  cle::Device::Pointer    device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

// 0 = EQUAL_CONSTANT
TEST_P(TestScalarComparison, equal_constant)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x == CONSTANT_VALUE; });

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::equal_constant_func(device, gpu_input, nullptr, CONSTANT_VALUE);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = GREATER_CONSTANT
TEST_P(TestScalarComparison, greater_constant)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x > CONSTANT_VALUE; });

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::greater_constant_func(device, gpu_input, nullptr, CONSTANT_VALUE);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 2 = SMALLER_CONSTANT
TEST_P(TestScalarComparison, smaller_constant)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x < CONSTANT_VALUE; });

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::smaller_constant_func(device, gpu_input, nullptr, CONSTANT_VALUE);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 3 = GREATER_OR_EQUAL_CONSTANT
TEST_P(TestScalarComparison, greater_or_equal_constant)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x >= CONSTANT_VALUE; });

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::greater_or_equal_constant_func(device, gpu_input, nullptr, CONSTANT_VALUE);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 4 = SMALLER_OR_EQUAL_CONSTANT
TEST_P(TestScalarComparison, smaller_or_equal_constant)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x <= CONSTANT_VALUE; });

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::smaller_or_equal_constant_func(device, gpu_input, nullptr, CONSTANT_VALUE);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 5 = NOT_EQUAL_CONSTANT
TEST_P(TestScalarComparison, not_equal_constant)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input.begin(), input.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input.begin(), input.end(), valid.begin(), [](const int8_t & x) { return x != CONSTANT_VALUE; });

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::not_equal_constant_func(device, gpu_input, nullptr, CONSTANT_VALUE);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}


INSTANTIATE_TEST_SUITE_P(InstantiationName, TestScalarComparison, ::testing::ValuesIn(getParameters()));
