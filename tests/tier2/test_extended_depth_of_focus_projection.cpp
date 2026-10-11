#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestExtendedDepthOfFocusProjection : public ::testing::TestWithParam<std::string>
{
protected:
  static constexpr size_t                   width = 8;
  static constexpr size_t                   height = 8;
  static constexpr size_t                   depth = 3;
  static constexpr size_t                   focus_slice = 1;
  std::string                               backend;
  cle::Device::Pointer                      device;
  std::array<float, width * height * depth> input;
  std::array<float, width * height>         valid;
  std::array<float, width * height>         output;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);

    // out-of-focus slices are flat (distinct constant per slice), the in-focus slice is textured
    unsigned int seed = 42;
    for (size_t z = 0; z < depth; z++)
    {
      for (size_t i = 0; i < width * height; i++)
      {
        float value = 10.0f * static_cast<float>(z + 1);
        if (z == focus_slice)
        {
          seed = seed * 1664525u + 1013904223u;
          value = static_cast<float>(seed >> 24);
          valid[i] = value;
        }
        input[z * width * height + i] = value;
      }
    }
  }
};

TEST_P(TestExtendedDepthOfFocusProjection, variance)
{
  auto gpu_input = cle::Array::create(width, height, depth, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier2::extended_depth_of_focus_variance_projection_func(device, gpu_input, nullptr, 1, 1, 2);

  EXPECT_EQ(gpu_output->width(), width);
  EXPECT_EQ(gpu_output->height(), height);
  EXPECT_EQ(gpu_output->depth(), 1);
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestExtendedDepthOfFocusProjection, sobel)
{
  auto gpu_input = cle::Array::create(width, height, depth, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::Array::create(width, height, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  cle::tier2::extended_depth_of_focus_sobel_projection_func(device, gpu_input, gpu_output, 2);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestExtendedDepthOfFocusProjection, ::testing::ValuesIn(getParameters()));
