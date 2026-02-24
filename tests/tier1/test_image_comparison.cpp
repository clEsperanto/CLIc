#include "cle.hpp"

#include <array>
#include <random>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated image comparison operations tests
// Tests: EQUAL, GREATER, SMALLER, GREATER_OR_EQUAL, SMALLER_OR_EQUAL, NOT_EQUAL
// Compares two images element-wise

class TestImageComparison : public ::testing::TestWithParam<std::string>
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

// 0 = EQUAL
TEST_P(TestImageComparison, equal)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a == b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::equal_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = GREATER
TEST_P(TestImageComparison, greater)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a > b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::greater_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 2 = SMALLER
TEST_P(TestImageComparison, smaller)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a < b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::smaller_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 3 = GREATER_OR_EQUAL
TEST_P(TestImageComparison, greater_or_equal)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a >= b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::greater_or_equal_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 4 = SMALLER_OR_EQUAL
TEST_P(TestImageComparison, smaller_or_equal)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a <= b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::smaller_or_equal_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 5 = NOT_EQUAL
TEST_P(TestImageComparison, not_equal)
{
  std::array<int8_t, 10 * 5 * 3> output;
  std::array<int8_t, 10 * 5 * 3> input1;
  std::array<int8_t, 10 * 5 * 3> input2;
  std::array<int8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(1, 10);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<int8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](const int8_t & a, const int8_t & b) { return a != b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::INT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::not_equal_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestImageComparison, ::testing::ValuesIn(getParameters()));
