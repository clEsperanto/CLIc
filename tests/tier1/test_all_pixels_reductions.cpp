#include "cle.hpp"

#include "test_utils.hpp"
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <numeric>
#include <string>
#include <vector>

// The reductions split the flattened image into at most 65536 partials: these shapes have more pixels than partials,
// a pixel count that is not a multiple of it, or a single row / column (no projection to share the work).
struct Shape
{
  size_t      width, height, depth, dim;
  cle::mType  mtype;
  std::string name;
};

class TestAllPixelsReductions : public ::testing::TestWithParam<std::string>
{
protected:
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    cle::BackendManager::getInstance().setBackend(GetParam());
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }

  static auto
  shapes() -> std::vector<Shape>
  {
    return { { 100003, 1, 1, 1, cle::mType::BUFFER, "1D" },
             { 1, 70001, 1, 2, cle::mType::BUFFER, "column" },
             { 61, 53, 29, 3, cle::mType::BUFFER, "3D odd" },
             { 300, 241, 1, 2, cle::mType::BUFFER, "2D" },
             { 300, 241, 1, 2, cle::mType::IMAGE, "2D image" } };
  }

  template <typename T>
  static auto
  values(size_t size, int low, int high) -> std::vector<T>
  {
    std::vector<T> data(size);
    for (size_t i = 0; i < size; ++i)
    {
      data[i] = static_cast<T>(low + static_cast<int>((i * 2654435761u) % static_cast<size_t>(high - low + 1)));
    }
    return data;
  }

  template <typename T>
  void
  check(cle::dType dtype, int low, int high)
  {
    for (const auto & s : shapes())
    {
      if (s.mtype == cle::mType::IMAGE && GetParam() != "opencl")
      {
        continue;
      }
      const auto data = values<T>(s.width * s.height * s.depth, low, high);
      auto       gpu = cle::Array::create(s.width, s.height, s.depth, s.dim, dtype, s.mtype, device);
      gpu->writeFrom(data.data());

      const double sum = std::accumulate(data.begin(), data.end(), 0.0);
      EXPECT_NEAR(cle::tier1::sum_of_all_pixels_func(device, gpu), sum, std::abs(sum) * 1e-6 + 1e-3) << s.name;
      EXPECT_EQ(cle::tier1::minimum_of_all_pixels_func(device, gpu), static_cast<float>(*std::min_element(data.begin(), data.end())))
        << s.name;
      EXPECT_EQ(cle::tier1::maximum_of_all_pixels_func(device, gpu), static_cast<float>(*std::max_element(data.begin(), data.end())))
        << s.name;
    }
  }
};

TEST_P(TestAllPixelsReductions, maximum)
{
  std::vector<float> input(10 * 20 * 30, 42.0F);
  input[(10 / 2) + (20 / 2) * 10 + (30 / 2) * 10 * 20] = 100.0F;
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->writeFrom(input.data());
  EXPECT_EQ(cle::tier1::maximum_of_all_pixels_func(device, array), 100.0F);
}

TEST_P(TestAllPixelsReductions, minimum)
{
  std::vector<float> input(10 * 20 * 30, 100.0F);
  input[(10 / 2) + (20 / 2) * 10 + (30 / 2) * 10 * 20] = 42.0F;
  auto array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->writeFrom(input.data());
  EXPECT_EQ(cle::tier1::minimum_of_all_pixels_func(device, array), 42.0F);
}

TEST_P(TestAllPixelsReductions, sum)
{
  std::vector<float> input(10 * 20 * 30, 1.0F);
  auto               array = cle::Array::create(10, 20, 30, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->writeFrom(input.data());
  EXPECT_EQ(cle::tier1::sum_of_all_pixels_func(device, array), 10 * 20 * 30);
}

TEST_P(TestAllPixelsReductions, product)
{
  std::vector<float> input(2 * 2 * 2, 2.0F);
  auto               array = cle::Array::create(2, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  array->writeFrom(input.data());
  EXPECT_EQ(cle::tier1::product_of_all_pixels_func(device, array), 256.0F); // 2^8
}

TEST_P(TestAllPixelsReductions, maskedMinimum)
{
  // the mask keeps every other pixel, the minimum (3) is on a masked pixel
  std::vector<float> input(5 * 3 * 2, 100.0F);
  std::vector<float> mask(5 * 3 * 2);
  for (size_t i = 0; i < mask.size(); i++)
  {
    mask[i] = (i % 2 == 0) ? 1 : 0;
  }
  input[2] = 3;
  auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_mask = cle::Array::create(gpu_input);
  gpu_input->writeFrom(input.data());
  gpu_mask->writeFrom(mask.data());
  EXPECT_EQ(cle::tier1::minimum_of_masked_pixels_func(device, gpu_input, gpu_mask), 3.0F);
}

TEST_P(TestAllPixelsReductions, uint8)
{
  check<uint8_t>(cle::dType::UINT8, 3, 250);
}

TEST_P(TestAllPixelsReductions, int16)
{
  check<int16_t>(cle::dType::INT16, -1000, 1000);
}

TEST_P(TestAllPixelsReductions, float32)
{
  check<float>(cle::dType::FLOAT, -500, 400);
}

TEST_P(TestAllPixelsReductions, productLarge)
{
  // 2, 0.5, 1, 2, 0.5, 1, ...: every partial product is exact in float, and a pixel missed or counted twice changes the
  // result by a factor 2. 100003 pixels: one more 2 than 0.5, the product is 2.
  std::vector<float> data(100003);
  for (size_t i = 0; i < data.size(); ++i)
  {
    data[i] = (i % 3 == 0) ? 2.0F : (i % 3 == 1) ? 0.5F : 1.0F;
  }

  auto gpu = cle::Array::create(data.size(), 1, 1, 1, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu->writeFrom(data.data());
  EXPECT_EQ(cle::tier1::product_of_all_pixels_func(device, gpu), 2.0F);
}

TEST_P(TestAllPixelsReductions, maskedMinimumLarge)
{
  for (const auto & s : shapes())
  {
    if (s.mtype == cle::mType::IMAGE && GetParam() != "opencl")
    {
      continue;
    }
    const size_t size = s.width * s.height * s.depth;
    const auto   data = values<float>(size, -500, 400);
    // the global minimum is outside the mask
    std::vector<uint8_t> mask(size);
    for (size_t i = 0; i < size; ++i)
    {
      mask[i] = (data[i] > -100) ? 1 : 0;
    }
    float valid = FLT_MAX;
    for (size_t i = 0; i < size; ++i)
    {
      valid = mask[i] ? std::min(valid, data[i]) : valid;
    }

    auto gpu = cle::Array::create(s.width, s.height, s.depth, s.dim, cle::dType::FLOAT, s.mtype, device);
    auto gpu_mask = cle::Array::create(s.width, s.height, s.depth, s.dim, cle::dType::UINT8, s.mtype, device);
    gpu->writeFrom(data.data());
    gpu_mask->writeFrom(mask.data());
    EXPECT_EQ(cle::tier1::minimum_of_masked_pixels_func(device, gpu, gpu_mask), valid) << s.name;
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestAllPixelsReductions, ::testing::ValuesIn(getParameters()));
