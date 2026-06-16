#include "cle.hpp"

#include "test_utils.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

class TestLabelsNeighborsStatistics : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  void
  SetUp() override
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }

  static auto
  get(const cle::StatisticsMap & stats, const std::string & key) -> const std::vector<float> &
  {
    auto it = stats.find(key);
    if (it == stats.end())
    {
      throw std::runtime_error("Missing key: " + key);
    }
    return it->second;
  }

  static auto
  max_of(const std::vector<float> & values) -> float
  {
    return *std::max_element(values.begin(), values.end());
  }

  template <size_t N>
  static auto
  upsample_labels_2d(const std::array<uint32_t, N> & src, int src_w, int src_h, int factor_x, int factor_y) -> std::vector<uint32_t>
  {
    std::vector<uint32_t> dst(src_w * factor_x * src_h * factor_y, 0);
    const int             dst_w = src_w * factor_x;
    for (int y = 0; y < src_h; ++y)
    {
      for (int x = 0; x < src_w; ++x)
      {
        const uint32_t v = src[static_cast<size_t>(y * src_w + x)];
        for (int yy = 0; yy < factor_y; ++yy)
        {
          for (int xx = 0; xx < factor_x; ++xx)
          {
            const int dx = x * factor_x + xx;
            const int dy = y * factor_y + yy;
            dst[static_cast<size_t>(dy * dst_w + dx)] = v;
          }
        }
      }
    }
    return dst;
  }
};

TEST_P(TestLabelsNeighborsStatistics, execute)
{
  // clang-format off
  const std::array<uint32_t, 8 * 8> labels = {
    1, 1, 2, 2, 3, 3, 3, 3,
    1, 1, 2, 2, 3, 3, 3, 3,
    1, 1, 7, 7, 7, 7, 3, 3,
    1, 1, 7, 7, 7, 7, 3, 3,
    6, 6, 7, 7, 7, 7, 4, 4,
    6, 6, 7, 7, 7, 7, 4, 4,
    5, 5, 5, 5, 5, 5, 4, 4,
    5, 5, 5, 5, 5, 5, 4, 4
  };
  // clang-format on

  auto scaled_labels_host = upsample_labels_2d(labels, 8, 8, 10, 10);
  auto scaled_labels = cle::Array::create(80, 80, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  scaled_labels->writeFrom(scaled_labels_host.data());

  std::vector<int> proximal_distances = { 20, 40 };
  std::vector<int> nearest_neighbor_ns = { 1, 2, 3, 4, 5, 6 };
  std::vector<int> dilation_radii = {};

  auto stats = cle::tier7::labels_neighbors_statistics_func(device, scaled_labels, proximal_distances, nearest_neighbor_ns, dilation_radii);

  const auto & labels_out = get(stats, "label");
  ASSERT_FALSE(labels_out.empty());
  EXPECT_NEAR(labels_out.back(), 7.0f, 0.001f);

  const auto & touching_neighbor_count = get(stats, "touching_neighbor_count");
  EXPECT_NEAR(touching_neighbor_count.back(), 6.0f, 0.001f);

  const auto & max_touching_dist = get(stats, "maximum_distance_of_touching_neighbors");
  EXPECT_NEAR(max_of(max_touching_dist), 43.843155f, 0.001f);
  EXPECT_NEAR(max_touching_dist.back(), 36.055511f, 0.001f);

  const auto & avg_touching_dist = get(stats, "average_distance_of_touching_neighbors");
  EXPECT_NEAR(avg_touching_dist.back(), 33.329613f, 0.001f);

  const auto & min_touching_dist = get(stats, "minimum_distance_of_touching_neighbors");
  EXPECT_NEAR(min_touching_dist.back(), 31.622776f, 0.001f);

  EXPECT_NEAR(get(stats, "proximal_neighbor_count_d20").back(), 0.0f, 0.001f);
  EXPECT_NEAR(get(stats, "proximal_neighbor_count_d40").back(), 6.0f, 0.001f);

  EXPECT_NEAR(get(stats, "maximum_distance_of_n1_nearest_neighbors").back(), 31.622776f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_distance_of_n2_nearest_neighbors").back(), 31.622776f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_distance_of_n3_nearest_neighbors").back(), 31.622776f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_distance_of_n4_nearest_neighbors").back(), 32.998318f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_distance_of_n5_nearest_neighbors").back(), 36.055511f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_distance_of_n6_nearest_neighbors").back(), 36.055511f, 0.001f);

  EXPECT_NEAR(get(stats, "touch_count_sum").back(), 160.0f, 0.001f);
  EXPECT_NEAR(get(stats, "minimum_touch_count").back(), 20.0f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_touch_count").back(), 40.0f, 0.001f);
  EXPECT_NEAR(get(stats, "minimum_touch_portion").back(), 0.125f, 0.001f);
  EXPECT_NEAR(get(stats, "maximum_touch_portion").back(), 0.25f, 0.001f);

  EXPECT_NEAR(get(stats, "touch_portion_above_0.000000_neighbor_count").back(), 6.0f, 0.001f);
  EXPECT_NEAR(get(stats, "touch_portion_above_0.160000_neighbor_count").back(), 2.0f, 0.001f);
  EXPECT_NEAR(get(stats, "touch_portion_above_0.200000_neighbor_count").back(), 2.0f, 0.001f);
  EXPECT_NEAR(get(stats, "touch_portion_above_0.330000_neighbor_count").back(), 0.0f, 0.001f);
  EXPECT_NEAR(get(stats, "touch_portion_above_0.500000_neighbor_count").back(), 0.0f, 0.001f);
  EXPECT_NEAR(get(stats, "touch_portion_above_0.750000_neighbor_count").back(), 0.0f, 0.001f);
}

TEST_P(TestLabelsNeighborsStatistics, dilated)
{
  // clang-format off
  const std::array<uint32_t, 8 * 8> labels = {
    0, 1, 2, 0, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 7, 0, 0, 0, 4, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 0, 0, 0, 0, 0, 0, 0
  };
  // clang-format on

  auto scaled_labels_host = upsample_labels_2d(labels, 8, 8, 2, 2);
  auto scaled_labels = cle::Array::create(16, 16, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  scaled_labels->writeFrom(scaled_labels_host.data());

  std::vector<int> proximal_distances = { 10, 20, 40, 80, 160 };
  std::vector<int> nearest_neighbor_ns = { 1, 2, 3, 4, 5, 6, 7, 8, 10, 20 };
  std::vector<int> dilation_radii = { 0, 10 };

  auto stats = cle::tier7::labels_neighbors_statistics_func(device, scaled_labels, proximal_distances, nearest_neighbor_ns, dilation_radii);

  const auto & touching_neighbor_count = get(stats, "touching_neighbor_count");
  const auto & minimum_distance_of_touching_neighbors = get(stats, "minimum_distance_of_touching_neighbors");
  const auto & average_distance_of_touching_neighbors = get(stats, "average_distance_of_touching_neighbors");
  const auto & maximum_distance_of_touching_neighbors = get(stats, "maximum_distance_of_touching_neighbors");
  const auto & max_min_distance_ratio_of_touching_neighbors = get(stats, "max_min_distance_ratio_of_touching_neighbors");
  const auto & touch_count_sum = get(stats, "touch_count_sum");
  const auto & minimum_touch_count = get(stats, "minimum_touch_count");
  const auto & maximum_touch_count = get(stats, "maximum_touch_count");
  const auto & minimum_touch_portion = get(stats, "minimum_touch_portion");
  const auto & maximum_touch_portion = get(stats, "maximum_touch_portion");

  const auto & touching_neighbor_count_r0 = get(stats, "touching_neighbor_count_dilated_r_0");
  const auto & min_dist_r0 = get(stats, "minimum_distance_of_touching_neighbors_dilated_r_0");
  const auto & avg_dist_r0 = get(stats, "average_distance_of_touching_neighbors_dilated_r_0");
  const auto & max_dist_r0 = get(stats, "maximum_distance_of_touching_neighbors_dilated_r_0");
  const auto & ratio_r0 = get(stats, "max_min_distance_ratio_of_touching_neighbors_dilated_r_0");
  const auto & touch_count_sum_r0 = get(stats, "touch_count_sum_dilated_r_0");
  const auto & minimum_touch_count_r0 = get(stats, "minimum_touch_count_dilated_r_0");
  const auto & maximum_touch_count_r0 = get(stats, "maximum_touch_count_dilated_r_0");
  const auto & minimum_touch_portion_r0 = get(stats, "minimum_touch_portion_dilated_r_0");
  const auto & maximum_touch_portion_r0 = get(stats, "maximum_touch_portion_dilated_r_0");

  ASSERT_EQ(touching_neighbor_count_r0.size(), touching_neighbor_count.size());
  for (size_t i = 0; i < touching_neighbor_count.size(); ++i)
  {
    EXPECT_EQ(touching_neighbor_count_r0[i], touching_neighbor_count[i]);
    EXPECT_EQ(min_dist_r0[i], minimum_distance_of_touching_neighbors[i]);
    EXPECT_EQ(avg_dist_r0[i], average_distance_of_touching_neighbors[i]);
    EXPECT_EQ(max_dist_r0[i], maximum_distance_of_touching_neighbors[i]);

    const bool r0_nan = std::isnan(ratio_r0[i]);
    const bool base_nan = std::isnan(max_min_distance_ratio_of_touching_neighbors[i]);
    EXPECT_TRUE((ratio_r0[i] == max_min_distance_ratio_of_touching_neighbors[i]) || (r0_nan && base_nan));

    EXPECT_EQ(touch_count_sum_r0[i], touch_count_sum[i]);
    EXPECT_EQ(minimum_touch_count_r0[i], minimum_touch_count[i]);
    EXPECT_EQ(maximum_touch_count_r0[i], maximum_touch_count[i]);
    EXPECT_EQ(minimum_touch_portion_r0[i], minimum_touch_portion[i]);
    EXPECT_EQ(maximum_touch_portion_r0[i], maximum_touch_portion[i]);
  }

  const auto & touching_neighbor_count_r10 = get(stats, "touching_neighbor_count_dilated_r_10");
  const auto & min_dist_r10 = get(stats, "minimum_distance_of_touching_neighbors_dilated_r_10");
  const auto & avg_dist_r10 = get(stats, "average_distance_of_touching_neighbors_dilated_r_10");
  const auto & max_dist_r10 = get(stats, "maximum_distance_of_touching_neighbors_dilated_r_10");
  const auto & ratio_r10 = get(stats, "max_min_distance_ratio_of_touching_neighbors_dilated_r_10");
  const auto & touch_count_sum_r10 = get(stats, "touch_count_sum_dilated_r_10");
  const auto & minimum_touch_count_r10 = get(stats, "minimum_touch_count_dilated_r_10");
  const auto & maximum_touch_count_r10 = get(stats, "maximum_touch_count_dilated_r_10");
  const auto & minimum_touch_portion_r10 = get(stats, "minimum_touch_portion_dilated_r_10");
  const auto & maximum_touch_portion_r10 = get(stats, "maximum_touch_portion_dilated_r_10");

  bool any_touching_count_gt = false;
  bool any_min_dist_gt = false;
  bool any_avg_dist_gt = false;
  bool any_max_dist_gt = false;
  bool any_ratio_gt = false;
  bool any_touch_count_sum_gt = false;
  bool any_min_touch_count_gt = false;
  bool any_max_touch_count_gt = false;
  bool any_min_touch_portion_ne = false;
  bool any_max_touch_portion_gt = false;

  for (size_t i = 0; i < touching_neighbor_count.size(); ++i)
  {
    any_touching_count_gt = any_touching_count_gt || (touching_neighbor_count_r10[i] > touching_neighbor_count[i]);
    any_min_dist_gt = any_min_dist_gt || (min_dist_r10[i] > minimum_distance_of_touching_neighbors[i]);
    any_avg_dist_gt = any_avg_dist_gt || (avg_dist_r10[i] > average_distance_of_touching_neighbors[i]);
    any_max_dist_gt = any_max_dist_gt || (max_dist_r10[i] > maximum_distance_of_touching_neighbors[i]);
    any_ratio_gt = any_ratio_gt || (ratio_r10[i] > max_min_distance_ratio_of_touching_neighbors[i]);
    any_touch_count_sum_gt = any_touch_count_sum_gt || (touch_count_sum_r10[i] > touch_count_sum[i]);
    any_min_touch_count_gt = any_min_touch_count_gt || (minimum_touch_count_r10[i] > minimum_touch_count[i]);
    any_max_touch_count_gt = any_max_touch_count_gt || (maximum_touch_count_r10[i] > maximum_touch_count[i]);
    any_min_touch_portion_ne = any_min_touch_portion_ne || (minimum_touch_portion_r10[i] != minimum_touch_portion[i]);
    any_max_touch_portion_gt = any_max_touch_portion_gt || (maximum_touch_portion_r10[i] > maximum_touch_portion[i]);
  }

  EXPECT_TRUE(any_touching_count_gt);
  EXPECT_TRUE(any_min_dist_gt);
  EXPECT_TRUE(any_avg_dist_gt);
  EXPECT_TRUE(any_max_dist_gt);
  EXPECT_TRUE(any_ratio_gt);
  EXPECT_TRUE(any_touch_count_sum_gt);
  EXPECT_TRUE(any_min_touch_count_gt);
  EXPECT_TRUE(any_max_touch_count_gt);
  EXPECT_TRUE(any_min_touch_portion_ne);
  EXPECT_TRUE(any_max_touch_portion_gt);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelsNeighborsStatistics, ::testing::ValuesIn(getParameters()));
