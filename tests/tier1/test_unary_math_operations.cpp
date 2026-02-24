#include "cle.hpp"

#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated unary math operations tests
// Tests: ABSOLUTE, CUBIC_ROOT, SQUARE_ROOT, SIGN

class TestUnaryMathOperations : public ::testing::TestWithParam<std::tuple<std::string, int>>
{
protected:
  int operation_type;
  std::string backend;

  virtual void
  SetUp()
  {
    backend = std::get<0>(GetParam());
    operation_type = std::get<1>(GetParam());
  }
};

// 0 = ABSOLUTE
TEST_P(TestUnaryMathOperations, absolute)
{
  if (operation_type != 0)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 1)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 2)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 3)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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

std::vector<std::tuple<std::string, int>>
generate_unary_math_test_params()
{
  std::vector<std::tuple<std::string, int>> params;
  std::vector<std::string> backends = getParameters();
  int operation_types[] = { 0, 1, 2, 3 };

  for (const auto& backend : backends)
  {
    for (int type : operation_types)
    {
      params.push_back(std::make_tuple(backend, type));
    }
  }
  return params;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestUnaryMathOperations,
                         ::testing::ValuesIn(generate_unary_math_test_params()));
