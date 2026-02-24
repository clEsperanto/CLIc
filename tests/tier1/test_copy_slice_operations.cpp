#include "cle.hpp"

#include <array>
#include <numeric>
#include <gtest/gtest.h>
#include "test_utils.hpp"

// Consolidated copy and slice operations tests
// Tests: COPY, COPY_SLICE, COPY_HORIZONTAL_SLICE, COPY_VERTICAL_SLICE

class TestCopySliceOperations : public ::testing::TestWithParam<std::tuple<std::string, int>>
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

// 0 = COPY
TEST_P(TestCopySliceOperations, copy)
{
  if (operation_type != 0)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<float, 10 * 5 * 3> output;
  std::array<float, 10 * 5 * 3> input;
  std::array<float, 10 * 5 * 3> valid;

  std::fill(input.begin(), input.end(), static_cast<float>(10));
  std::fill(valid.begin(), valid.end(), static_cast<float>(10));

  auto gpu_input = cle::Array::create(10, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::copy_func(device, gpu_input, nullptr);
  gpu_output->readTo(output.data());

  for (size_t i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

// 1 = COPY_SLICE
TEST_P(TestCopySliceOperations, copy_slice)
{
  if (operation_type != 1)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<float, 2 * 2 * 1> output;
  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };

  auto gpu_input = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 4.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.25);
}

// 2 = COPY_HORIZONTAL_SLICE
TEST_P(TestCopySliceOperations, copy_horizontal_slice)
{
  if (operation_type != 2)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };
  std::array<float, 2 * 2 * 1> output;

  auto gpu_input = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_horizontal_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 1.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 4.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 2.25);
}

// 3 = COPY_VERTICAL_SLICE
TEST_P(TestCopySliceOperations, copy_vertical_slice)
{
  if (operation_type != 3)
    GTEST_SKIP();

  cle::BackendManager::getInstance().setBackend(backend);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<float, 2 * 2 * 2> input = { 1, 4, 0, 4, 1, 3, 1, 2 };
  std::array<float, 2 * 2 * 1> output;

  auto gpu_input = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_output = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  gpu_output->fill(0);

  cle::tier1::copy_vertical_slice_func(device, gpu_input, gpu_output, 0);

  gpu_output->readTo(output.data());
  EXPECT_EQ(*std::min_element(output.begin(), output.end()), 0.0);
  EXPECT_EQ(*std::max_element(output.begin(), output.end()), 1.0);
  EXPECT_EQ(std::accumulate(output.begin(), output.end(), 0.0) / output.size(), 0.75);
}

std::vector<std::tuple<std::string, int>>
generate_copy_slice_test_params()
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCopySliceOperations,
                         ::testing::ValuesIn(generate_copy_slice_test_params()));
