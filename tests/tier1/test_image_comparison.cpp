#include "cle.hpp"

#include <array>
#include <random>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated image comparison operations tests
// Tests: EQUAL, GREATER, SMALLER, GREATER_OR_EQUAL, SMALLER_OR_EQUAL, NOT_EQUAL
// Compares two images element-wise

class TestImageComparison : public ::testing::TestWithParam<std::tuple<std::string, int>>
{
protected:
  int comparison_type;
  std::string backend;

  virtual void
  SetUp()
  {
    backend = std::get<0>(GetParam());
    comparison_type = std::get<1>(GetParam());
  }
};

// 0 = EQUAL
TEST_P(TestImageComparison, equal)
{
  if (comparison_type != 0)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 1)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 2)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 3)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 4)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (comparison_type != 5)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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

std::vector<std::tuple<std::string, int>>
generate_image_comparison_test_params()
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestImageComparison,
                         ::testing::ValuesIn(generate_image_comparison_test_params()));
