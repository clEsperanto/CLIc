#include "cle.hpp"

#include "test_utils.hpp"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <gtest/gtest.h>
#include <random>
#include <vector>

class TestLabeling : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                           backend;
  cle::Device::Pointer                  device;
  const std::array<uint32_t, 5 * 3 * 2> valid_diamond = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                          2, 0, 0, 1, 0, 0, 3, 0, 1, 1, 0, 3, 0, 0, 0 };
  const std::array<uint32_t, 5 * 3 * 2> valid_box = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                      2, 0, 0, 1, 0, 0, 2, 0, 1, 1, 0, 2, 0, 0, 0 };
  const std::array<uint32_t, 5 * 3 * 2> input = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0
  };
  std::array<uint32_t, 5 * 3 * 2> output;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestLabeling, executeBox)
{
  auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::connected_component_labeling_func(device, gpu_input, nullptr, "box");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_box[i]);
  }
}

TEST_P(TestLabeling, executeDiamond)
{
  auto gpu_input = cle::Array::create(5, 3, 2, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::connected_component_labeling_func(device, gpu_input, nullptr, "sphere");

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid_diamond[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabeling, ::testing::ValuesIn(getParameters()));


// Host reference: flood fill, components numbered in scan order of their first pixel (= relabel_sequential order)
static auto
referenceLabeling(const std::vector<uint8_t> & binary, int w, int h, int d, bool diamond) -> std::vector<uint32_t>
{
  std::vector<uint32_t> labels(binary.size(), 0);
  std::vector<size_t>   stack;
  uint32_t              next = 0;
  for (size_t start = 0; start < binary.size(); ++start)
  {
    if (binary[start] == 0 || labels[start] != 0)
    {
      continue;
    }
    labels[start] = ++next;
    stack.push_back(start);
    while (!stack.empty())
    {
      const size_t i = stack.back();
      stack.pop_back();
      const int x = static_cast<int>(i % w);
      const int y = static_cast<int>((i / w) % h);
      const int z = static_cast<int>(i / (static_cast<size_t>(w) * h));
      for (int dz = -1; dz <= 1; ++dz)
      {
        for (int dy = -1; dy <= 1; ++dy)
        {
          for (int dx = -1; dx <= 1; ++dx)
          {
            const int manhattan = std::abs(dx) + std::abs(dy) + std::abs(dz);
            const int nx = x + dx, ny = y + dy, nz = z + dz;
            if (manhattan == 0 || (diamond && manhattan != 1) || nx < 0 || nx >= w || ny < 0 || ny >= h || nz < 0 || nz >= d)
            {
              continue;
            }
            const size_t n = (static_cast<size_t>(nz) * h + ny) * w + nx;
            if (binary[n] != 0 && labels[n] == 0)
            {
              labels[n] = next;
              stack.push_back(n);
            }
          }
        }
      }
    }
  }
  return labels;
}

class TestLabelingReference : public ::testing::TestWithParam<std::string>
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

  void
  check(const std::vector<uint8_t> & binary, int w, int h, int d, int dim)
  {
    for (const std::string connectivity : { "box", "sphere" })
    {
      auto gpu_input = cle::Array::create(w, h, d, dim, cle::dType::UINT8, cle::mType::BUFFER, device);
      gpu_input->writeFrom(binary.data());
      auto gpu_output = cle::tier5::connected_component_labeling_func(device, gpu_input, nullptr, connectivity);
      EXPECT_EQ(gpu_output->dtype(), cle::dType::LABEL);

      std::vector<uint32_t> output(binary.size());
      gpu_output->readTo(output.data());
      const auto valid = referenceLabeling(binary, w, h, d, connectivity == "sphere");
      size_t     mismatches = 0;
      for (size_t i = 0; i < output.size(); ++i)
      {
        mismatches += (output[i] != valid[i]);
      }
      EXPECT_EQ(mismatches, 0u) << "connectivity " << connectivity << ", shape " << w << "x" << h << "x" << d;
    }
  }

  static auto
  random(size_t size, float density, unsigned seed) -> std::vector<uint8_t>
  {
    std::mt19937                rng(seed);
    std::bernoulli_distribution foreground(density);
    std::vector<uint8_t>        binary(size);
    for (auto & v : binary)
    {
      v = foreground(rng) ? 1 : 0;
    }
    return binary;
  }
};

TEST_P(TestLabelingReference, random2D)
{
  // densities around the percolation threshold give long, branching components and contended merges
  for (const float density : { 0.3F, 0.5F, 0.6F, 0.75F })
  {
    check(random(256 * 256, density, 1), 256, 256, 1, 2);
  }
}

TEST_P(TestLabelingReference, random3D)
{
  for (const float density : { 0.15F, 0.3F, 0.5F })
  {
    check(random(64 * 48 * 40, density, 2), 64, 48, 40, 3);
  }
}

TEST_P(TestLabelingReference, random1D)
{
  check(random(1000, 0.5F, 3), 1000, 1, 1, 1);
}

TEST_P(TestLabelingReference, serpentine)
{
  // a single path winding through the whole image: the worst case for iterative label propagation
  const int            w = 128, h = 128;
  std::vector<uint8_t> binary(w * h, 0);
  for (int y = 0; y < h; y += 2)
  {
    std::fill_n(binary.begin() + y * w, w, 1);
    if (y + 1 < h)
    {
      binary[(y + 1) * w + ((y / 2) % 2 ? 0 : w - 1)] = 1;
    }
  }
  check(binary, w, h, 1, 2);
}

TEST_P(TestLabelingReference, emptyAndFull)
{
  check(std::vector<uint8_t>(32 * 16 * 8, 0), 32, 16, 8, 3);
  check(std::vector<uint8_t>(32 * 16 * 8, 1), 32, 16, 8, 3);
}

TEST_P(TestLabelingReference, pixelIndexAbove2pow24)
{
  // pixel indices above 2^24 are not exact in float: the initial labels 4k-1 and 4k+1 (pixels 4k-2 and 4k, not
  // touching) both round to 4k, which merged the two components when the index was computed in float
  const int            w = 4100, h = 4100;
  std::vector<uint8_t> binary(static_cast<size_t>(w) * h, 0);
  binary[binary.size() - 4] = 1; // w * h is a multiple of 4
  binary[binary.size() - 6] = 1;
  check(binary, w, h, 1, 2);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelingReference, ::testing::ValuesIn(getParameters()));
