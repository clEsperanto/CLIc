#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <cmath>
#include <gtest/gtest.h>

class TestKuwahara : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestKuwahara, constantImageIsUnchanged)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  // On a flat image every region has zero variance, so the variance-weighted
  // blend must return the input value unchanged, regardless of radius/sigma.
  constexpr int            size = 7 * 7;
  std::array<float, size>  input;
  std::array<float, size>  output;
  std::fill(input.begin(), input.end(), 42.0F);

  auto gpu_input = cle::Array::create(7, 7, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier1::kuwahara_filter_func(device, gpu_input, nullptr, 2, 1.5F);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], 42.0F, 0.001F);
  }
}

TEST_P(TestKuwahara, preservesStepEdgeBetterThanGaussianBlur)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  // Sharp vertical step edge: left half low, right half high.
  constexpr int                     width = 10;
  constexpr int                     height = 10;
  constexpr float                   low = 0.0F;
  constexpr float                   high = 100.0F;
  std::array<float, width * height> input;
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      input[y * width + x] = (x < width / 2) ? low : high;
    }
  }

  auto gpu_input = cle::Array::create(width, height, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_kuwahara = cle::tier1::kuwahara_filter_func(device, gpu_input, nullptr, 2, 1.5F);
  auto gpu_gaussian = cle::tier1::gaussian_blur_func(device, gpu_input, nullptr, 2.0F, 2.0F, 0.0F);

  std::array<float, width * height> kuwahara_output;
  std::array<float, width * height> gaussian_output;
  gpu_kuwahara->readTo(kuwahara_output.data());
  gpu_gaussian->readTo(gaussian_output.data());

  // Right at the edge, the Kuwahara filter should stay closer to one of the
  // two original intensities than an isotropic Gaussian blur of comparable
  // extent, since it picks the lowest-variance (homogeneous) region.
  const int edge_index_left = height / 2 * width + (width / 2 - 1);
  const int edge_index_right = height / 2 * width + (width / 2);

  const float kuwahara_left_error =
    std::min(std::abs(kuwahara_output[edge_index_left] - low), std::abs(kuwahara_output[edge_index_left] - high));
  const float gaussian_left_error =
    std::min(std::abs(gaussian_output[edge_index_left] - low), std::abs(gaussian_output[edge_index_left] - high));
  const float kuwahara_right_error =
    std::min(std::abs(kuwahara_output[edge_index_right] - low), std::abs(kuwahara_output[edge_index_right] - high));
  const float gaussian_right_error =
    std::min(std::abs(gaussian_output[edge_index_right] - low), std::abs(gaussian_output[edge_index_right] - high));

  EXPECT_LE(kuwahara_left_error, gaussian_left_error);
  EXPECT_LE(kuwahara_right_error, gaussian_right_error);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestKuwahara, ::testing::ValuesIn(getParameters()));
