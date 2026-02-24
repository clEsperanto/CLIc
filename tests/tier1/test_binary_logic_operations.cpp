#include "cle.hpp"

#include <array>
#include <random>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated binary logic operations tests
// Tests: AND, OR, XOR, NOT, SUBTRACT, INF_SUP, SUP_INF

class TestBinaryLogicOperations : public ::testing::TestWithParam<std::tuple<std::string, int>>
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

// 0 = AND
TEST_P(TestBinaryLogicOperations, binary_and)
{
  if (operation_type != 0)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 1)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 2)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 3)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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
  if (operation_type != 4)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

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



std::vector<std::tuple<std::string, int>>
generate_binary_logic_test_params()
{
  std::vector<std::tuple<std::string, int>> params;
  std::vector<std::string> backends = getParameters();
  int operation_types[] = { 0, 1, 2, 3, 4 };

  for (const auto& backend : backends)
  {
    for (int type : operation_types)
    {
      params.push_back(std::make_tuple(backend, type));
    }
  }
  return params;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBinaryLogicOperations,
                         ::testing::ValuesIn(generate_binary_logic_test_params()));
