#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <cmath>
#include <gtest/gtest.h>

class TestFilterLabelByValues : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                 backend;
  cle::Device::Pointer        device;
  std::array<uint32_t, 6 * 2> input = { 1, 2, 3, 4, 5, 6, 0, 6, 5, 4, 3, 2 };
  // one value per label, index 0 is the background (always kept); label 4 has no value (NaN, never removed)
  std::array<float, 7> values = { 100, 1, 2, 3, NAN, 5, 7 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }

  // expected output: input relabelled with new_labels[old label]
  auto
  relabel(const std::array<uint32_t, 7> & new_labels) const -> std::array<uint32_t, 6 * 2>
  {
    std::array<uint32_t, 6 * 2> valid{};
    for (size_t i = 0; i < input.size(); i++)
    {
      valid[i] = new_labels[input[i]];
    }
    return valid;
  }

  auto
  read(const cle::Array::Pointer & gpu_output) const -> std::array<uint32_t, 6 * 2>
  {
    std::array<uint32_t, 6 * 2> output{};
    gpu_output->readTo(output.data());
    return output;
  }
};

TEST_P(TestFilterLabelByValues, outOfRange)
{
  auto gpu_input = cle::Array::create(6, 2, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_values = cle::Array::create(values.size(), 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_values->writeFrom(values.data());

  // removed: value < 2 or value > 5 (the bounds are kept)
  auto gpu_output = cle::tier4::remove_labels_with_map_values_out_of_range_func(device, gpu_input, gpu_values, nullptr, 2, 5);
  EXPECT_EQ(read(gpu_output), relabel({ 0, 0, 1, 2, 3, 4, 0 }));
  gpu_output = cle::tier4::exclude_labels_with_map_values_out_of_range_func(device, gpu_values, gpu_input, nullptr, 2, 5);
  EXPECT_EQ(read(gpu_output), relabel({ 0, 0, 1, 2, 3, 4, 0 }));
}

TEST_P(TestFilterLabelByValues, withinRange)
{
  auto gpu_input = cle::Array::create(6, 2, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_values = cle::Array::create(values.size(), 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_values->writeFrom(values.data());

  // removed: 2 <= value <= 5 (the bounds are removed)
  auto gpu_output = cle::tier4::remove_labels_with_map_values_within_range_func(device, gpu_input, gpu_values, nullptr, 2, 5);
  EXPECT_EQ(read(gpu_output), relabel({ 0, 1, 0, 0, 2, 0, 3 }));
  gpu_output = cle::tier4::exclude_labels_with_map_values_within_range_func(device, gpu_values, gpu_input, nullptr, 2, 5);
  EXPECT_EQ(read(gpu_output), relabel({ 0, 1, 0, 0, 2, 0, 3 }));
}

TEST_P(TestFilterLabelByValues, integerValues)
{
  auto gpu_input = cle::Array::create(6, 2, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  const std::array<uint32_t, 7> int_values = { 0, 10, 20, 30, 40, 50, 60 };
  auto                          gpu_values = cle::Array::create(int_values.size(), 1, 1, 1, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_values->writeFrom(int_values.data());

  auto gpu_output = cle::tier4::remove_labels_with_map_values_out_of_range_func(device, gpu_input, gpu_values, nullptr, 20, 40);
  EXPECT_EQ(read(gpu_output), relabel({ 0, 0, 1, 2, 3, 0, 0 }));
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestFilterLabelByValues, ::testing::ValuesIn(getParameters()));
