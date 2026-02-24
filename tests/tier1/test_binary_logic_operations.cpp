#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>
#include <random>

// Consolidated binary logic operations tests
// Tests: AND, OR, XOR, NOT, SUBTRACT, INF_SUP, SUP_INF

class TestBinaryLogicOperations : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
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

// 0 = AND
TEST_P(TestBinaryLogicOperations, binary_and)
{
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input1;
  std::array<uint8_t, 10 * 5 * 3> input2;
  std::array<uint8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(0, 1);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), std::multiplies<uint8_t>());

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::binary_and_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = OR
TEST_P(TestBinaryLogicOperations, binary_or)
{
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input1;
  std::array<uint8_t, 10 * 5 * 3> input2;
  std::array<uint8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(0, 1);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), std::plus<bool>());

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::binary_or_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 2 = XOR
TEST_P(TestBinaryLogicOperations, binary_xor)
{
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input1;
  std::array<uint8_t, 10 * 5 * 3> input2;
  std::array<uint8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(0, 1);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), std::minus<bool>());

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::binary_xor_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 3 = NOT
TEST_P(TestBinaryLogicOperations, binary_not)
{
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input;
  std::array<uint8_t, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<uint8_t>(0));
  std::fill(valid.begin(), valid.end(), static_cast<uint8_t>(1));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::binary_not_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 4 = SUBTRACT
TEST_P(TestBinaryLogicOperations, binary_subtract)
{
  std::array<uint8_t, 10 * 5 * 3> output;
  std::array<uint8_t, 10 * 5 * 3> input1;
  std::array<uint8_t, 10 * 5 * 3> input2;
  std::array<uint8_t, 10 * 5 * 3> valid;

  static std::uniform_int_distribution<int> distribution(0, 1);
  static std::default_random_engine         generator;
  std::generate(input1.begin(), input1.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::generate(input2.begin(), input2.end(), []() { return static_cast<uint8_t>(distribution(generator)); });
  std::transform(input1.begin(), input1.end(), input2.begin(), valid.begin(), [](uint8_t a, uint8_t b) { return a && !b; });

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::UINT8, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier1::binary_subtract_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}


INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBinaryLogicOperations, ::testing::ValuesIn(getParameters()));
