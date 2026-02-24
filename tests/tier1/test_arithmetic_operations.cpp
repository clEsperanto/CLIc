#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated arithmetic operations tests
// Tests: ADD_IMAGE_AND_SCALAR, SUBTRACT_IMAGE_AND_SCALAR, MULTIPLY_IMAGE_AND_SCALAR,
//        MULTIPLY_IMAGES, DIVIDE_IMAGES, DIVIDE_SCALAR_BY_IMAGE,
//        ADD_IMAGE_WEIGHTED, MULTIPLY_IMAGE_AND_POSITION

class TestArithmeticOperations : public ::testing::TestWithParam<std::tuple<std::string, int>>
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

// 0 = ADD_IMAGE_AND_SCALAR
TEST_P(TestArithmeticOperations, add_image_and_scalar)
{
  if (operation_type != 0)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   scalar = 5;
  const float                   value = 10;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(value));
  std::fill(valid.begin(), valid.end(), static_cast<float>(value + scalar));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::add_image_and_scalar_func(device, gpu_input, nullptr, scalar);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = SUBTRACT_IMAGE_AND_SCALAR
TEST_P(TestArithmeticOperations, subtract_image_and_scalar)
{
  if (operation_type != 1)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   scalar = 5;
  const float                   value = 10;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(value));
  std::fill(valid.begin(), valid.end(), static_cast<float>(value - scalar));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::subtract_image_from_scalar_func(device, gpu_input, nullptr, scalar);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 2 = MULTIPLY_IMAGE_AND_SCALAR
TEST_P(TestArithmeticOperations, multiply_image_and_scalar)
{
  if (operation_type != 2)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   scalar = 5;
  const float                   value = 10;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(value));
  std::fill(valid.begin(), valid.end(), static_cast<float>(value * scalar));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::multiply_image_and_scalar_func(device, gpu_input, nullptr, scalar);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 3 = MULTIPLY_IMAGES
TEST_P(TestArithmeticOperations, multiply_images)
{
  if (operation_type != 3)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   value_1 = 25;
  const float                   value_2 = 75;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input1.begin(), input1.end(), value_1);
  std::fill(input2.begin(), input2.end(), value_2);
  std::fill(valid.begin(), valid.end(), value_1 * value_2);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());
  auto gpu_output = cle::tier1::multiply_images_func(device, gpu_input1, gpu_input2, nullptr);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 4 = DIVIDE_IMAGES
TEST_P(TestArithmeticOperations, divide_images)
{
  if (operation_type != 4)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   value_1 = 10;
  const float                   value_2 = 2;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input1.begin(), input1.end(), value_1);
  std::fill(input2.begin(), input2.end(), value_2);
  std::fill(valid.begin(), valid.end(), value_1 / value_2);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());
  auto gpu_output = cle::tier1::divide_images_func(device, gpu_input1, gpu_input2, nullptr);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 5 = DIVIDE_SCALAR_BY_IMAGE
TEST_P(TestArithmeticOperations, divide_scalar_by_image)
{
  if (operation_type != 5)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   scalar = 10;
  const float                   value = 2;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(value));
  std::fill(valid.begin(), valid.end(), static_cast<float>(scalar / value));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::divide_scalar_by_image_func(device, gpu_input, nullptr, scalar);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 6 = ADD_IMAGE_WEIGHTED
TEST_P(TestArithmeticOperations, add_image_weighted)
{
  if (operation_type != 6)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  const float                   factor1 = 2;
  const float                   factor2 = 3;
  const float                   value_1 = 5;
  const float                   value_2 = 4;
  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input1;
  std::array<float, 10 * 5 * 3> input2;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input1.begin(), input1.end(), value_1);
  std::fill(input2.begin(), input2.end(), value_2);
  std::fill(valid.begin(), valid.end(), factor1 * value_1 + factor2 * value_2);

  auto gpu_input1 = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());
  auto gpu_output = cle::tier1::add_images_weighted_func(device, gpu_input1, gpu_input2, nullptr, factor1, factor2);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 7 = MULTIPLY_IMAGE_AND_POSITION
TEST_P(TestArithmeticOperations, multiply_image_and_position)
{
  if (operation_type != 7)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<float, 3 * 3 * 1> output;
  std::array<float, 3 * 3 * 1> input;
  std::array<float, 3 * 3 * 1> valid = { 0, 1, 4, 0, 1, 4, 0, 1, 4 };

  std::fill(input.begin(), input.end(), static_cast<float>(1));

  auto gpu_input = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::multiply_image_and_position_func(device, gpu_input, nullptr, 0);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

std::vector<std::tuple<std::string, int>>
generate_arithmetic_operations_test_params()
{
  std::vector<std::tuple<std::string, int>> params;
  std::vector<std::string> backends = getParameters();
  int operation_types[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

  for (const auto& backend : backends)
  {
    for (int type : operation_types)
    {
      params.push_back(std::make_tuple(backend, type));
    }
  }
  return params;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestArithmeticOperations,
                         ::testing::ValuesIn(generate_arithmetic_operations_test_params()));
