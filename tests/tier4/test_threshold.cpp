
#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>
#include <string>
#include <tuple>

class TestThreshold : public ::testing::TestWithParam<std::string>
{
protected:
};

TEST_P(TestThreshold, otsu)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 6.0058594 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };    // 6.00392 clic
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestThreshold, otsuLowIntensity)
{
  const std::array<float, 3 * 3 * 1>   input_low = { 0, 0, 0, 0, 0.003, 0, 0, 0, 0 };
  const std::array<uint8_t, 3 * 3 * 1> valid_low = { 0, 0, 0, 0, 1, 0, 0, 0, 0 };
  std::array<uint8_t, 3 * 3 * 1>       output_low;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_low.data());

  auto gpu_output = cle::tier4::threshold_otsu_func(device, gpu_input, nullptr);

  gpu_output->readTo(output_low.data());
  for (int i = 0; i < output_low.size(); i++)
  {
    EXPECT_EQ(output_low[i], valid_low[i]);
  }
}

TEST_P(TestThreshold, yen)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 6.0058594 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };    // 6.00392 clic
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_yen_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
TEST_P(TestThreshold, triangle)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 1.0644531 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_triangle_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestThreshold, isodata)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 5.9628906 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 };
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_isodata_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestThreshold, li)
{
  const std::array<float, 3 * 2 * 2>   input = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 }; // 5.328085 skimage
  const std::array<uint8_t, 3 * 2 * 2> valid = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 };
  std::array<uint8_t, 3 * 2 * 2>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::threshold_li_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestThreshold, integerImageNarrowRange)
{
  // uint8 image with values from 20 to 140: fewer intensity values than histogram bins. Thresholds from scikit-image
  // 0.24, which uses one bin per intensity value for integer images. With 256 bins over this range, some bins could
  // not receive any pixel, and the triangle method picked one of these empty bins (threshold 36.5 instead of 56).
  const std::array<uint8_t, 8 * 8> input = { 20, 140, 36, 29, 25, 40,  39, 35, 46, 29, 36,  47, 37, 43, 26,  38, 43, 44, 49, 48, 37, 43,
                                             48, 46,  46, 39, 36, 49,  48, 54, 50, 35, 36,  33, 44, 38, 32,  45, 38, 42, 34, 45, 41, 47,
                                             37, 31,  43, 50, 51, 112, 51, 55, 50, 99, 107, 64, 57, 67, 134, 52, 54, 48, 71, 53 };
  using ThresholdFunction = cle::Array::Pointer (*)(const cle::Device::Pointer &, const cle::Array::Pointer &, cle::Array::Pointer);
  const std::array<std::tuple<std::string, ThresholdFunction, float>, 6> methods = {
    std::make_tuple("otsu", cle::tier4::threshold_otsu_func, 71.0F),
    std::make_tuple("yen", cle::tier4::threshold_yen_func, 51.0F),
    std::make_tuple("isodata", cle::tier4::threshold_isodata_func, 80.0F),
    std::make_tuple("triangle", cle::tier4::threshold_triangle_func, 56.0F),
    std::make_tuple("li", cle::tier4::threshold_li_func, 66.63187F),
    std::make_tuple("mean", cle::tier4::threshold_mean_func, 49.09375F)
  };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(8, 8, 1, 2, cle::dType::UINT8, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  for (const auto & [name, threshold_function, threshold] : methods)
  {
    auto                       gpu_output = threshold_function(device, gpu_input, nullptr);
    std::array<uint8_t, 8 * 8> output;
    gpu_output->readTo(output.data());
    for (int i = 0; i < output.size(); i++)
    {
      EXPECT_EQ(output[i], input[i] > threshold ? 1 : 0) << name << ", pixel " << i << " (" << int(input[i]) << ")";
    }
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestThreshold, ::testing::ValuesIn(getParameters()));
