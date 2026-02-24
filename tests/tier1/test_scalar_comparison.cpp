#include "cle.hpp"

#include <array>
#include <random>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated scalar comparison operations tests
// Tests: EQUAL_CONSTANT, GREATER_CONSTANT, SMALLER_CONSTANT, 
// GREATER_OR_EQUAL_CONSTANT, SMALLER_OR_EQUAL_CONSTANT, NOT_EQUAL_CONSTANT
// Compares image with a scalar constant

class TestScalarComparison : public ::testing::TestWithParam<std::tuple<std::string, int>>
{
protected:
  int comparison_type;
  std::string backend;
  static constexpr int8_t CONSTANT_VALUE = 5;

  virtual void
  SetUp()
  {
    backend = std::get<0>(GetParam());
    comparison_type = std::get<1>(GetParam());
  }
};

// 0 = EQUAL_CONSTANT
TEST_P(TestScalarComparison, equal_constant)
{
  if (comparison_type != 0)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 1)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 2)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 3)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 4)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 5)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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

std::vector<std::tuple<std::string, int>>
generate_scalar_comparison_test_params()
{
  std::vector<std::tuple<std::string, int>> params;
  std::vector<std::string> backends = getParameters();
  int comparison_types[] = { 0, 1, 2, 3, 4, 5 };

  for (const auto& backend : backends)
  {
    for (int type : comparison_types)
    {
      params.push_back(std::make_tuple(backend, type));
    }
  }
  return params;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestScalarComparison,
                         ::testing::ValuesIn(generate_scalar_comparison_test_params()));
