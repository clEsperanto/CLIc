#include "cle.hpp"

#include "test_utils.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <set>
#include <vector>

class TestArtificialTissue : public ::testing::TestWithParam<std::string>
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

// The point positions are random, so only check properties that do not depend on the noise:
// a small sigma (8 sigma below half the spacing) keeps every seed on its own pixel, so each seed
// produces exactly one label, and the voronoi extension leaves no background pixel.

TEST_P(TestArtificialTissue, execute2D)
{
  // rows at y = 0, 8, ..., 56 (8 rows) with 8 points each (odd rows shifted by delta_x / 2)
  const size_t width = 64, height = 64, depth = 1;
  const size_t nb_points = 8 * 8;

  auto gpu_output = cle::tier3::artificial_tissue_func(device, width, height, depth, 8, 8, 1, 0.5, 0.5, 0.5);

  EXPECT_EQ(gpu_output->width(), width);
  EXPECT_EQ(gpu_output->height(), height);
  EXPECT_EQ(gpu_output->depth(), depth);
  EXPECT_EQ(gpu_output->dimension(), 2);
  EXPECT_EQ(gpu_output->dtype(), cle::dType::LABEL);

  std::vector<uint32_t> output(width * height * depth);
  gpu_output->readTo(output.data());
  const std::set<uint32_t> labels(output.begin(), output.end());
  EXPECT_EQ(labels.size(), nb_points);
  EXPECT_EQ(*labels.begin(), 1);
  EXPECT_EQ(*labels.rbegin(), nb_points);
}

TEST_P(TestArtificialTissue, execute3D)
{
  // 4 planes of 4 rows of 4 points
  const size_t width = 16, height = 16, depth = 16;
  const size_t nb_points = 4 * 4 * 4;

  auto gpu_output = cle::tier3::artificial_tissue_func(device, width, height, depth, 4, 4, 4, 0.2, 0.2, 0.2);

  EXPECT_EQ(gpu_output->width(), width);
  EXPECT_EQ(gpu_output->height(), height);
  EXPECT_EQ(gpu_output->depth(), depth);
  EXPECT_EQ(gpu_output->dimension(), 3);

  std::vector<uint32_t> output(width * height * depth);
  gpu_output->readTo(output.data());
  const std::set<uint32_t> labels(output.begin(), output.end());
  EXPECT_EQ(labels.size(), nb_points);
  EXPECT_EQ(*labels.begin(), 1);
  EXPECT_EQ(*labels.rbegin(), nb_points);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestArtificialTissue, ::testing::ValuesIn(getParameters()));
