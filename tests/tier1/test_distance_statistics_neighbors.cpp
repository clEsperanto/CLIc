#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestDistanceNeighbors : public ::testing::TestWithParam<std::string>
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

TEST_P(TestDistanceNeighbors, executeMeanDistanceTouching)
{
  // 6x6 label image: label 1 top-left, label 3 top-right, label 2 bottom-right
  // Label 1 and 3 touch along the vertical border; label 2 and 3 touch along the horizontal border.
  // Centroids: label1=(1,1), label2=(4,4), label3=(4,1)
  // Touching distances: d(1,3)=3, d(2,3)=3  -> mean per label: [0, 3, 3, 3]
  // clang-format off
  std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2
  };
  // clang-format on
  std::array<float, 4> valid = { 0.0f, 3.0f, 3.0f, 3.0f };

  auto gpu_labels = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(input.data());

  auto centroids = cle::tier4::centroids_of_labels_func(device, gpu_labels, nullptr, false);
  auto distance_mat = cle::tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto touch_mat = cle::tier3::generate_touch_matrix_func(device, gpu_labels, nullptr);
  auto result = cle::tier1::mean_distance_touching_neighbors_func(device, distance_mat, touch_mat, nullptr);

  std::array<float, 4> output;
  result->readTo(output.data());

  for (int i = 0; i < 4; i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01f);
  }
}

TEST_P(TestDistanceNeighbors, executeMinimumDistanceTouching)
{
  // Same label image as mean test. Each label has only one touching neighbor at distance 3,
  // so minimum equals mean: [0, 3, 3, 3]
  // clang-format off
  std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2
  };
  // clang-format on
  std::array<float, 4> valid = { 0.0f, 3.0f, 3.0f, 3.0f };

  auto gpu_labels = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(input.data());

  auto centroids = cle::tier4::centroids_of_labels_func(device, gpu_labels, nullptr, false);
  auto distance_mat = cle::tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto touch_mat = cle::tier3::generate_touch_matrix_func(device, gpu_labels, nullptr);
  auto result = cle::tier1::minimum_distance_touching_neighbors_func(device, distance_mat, touch_mat, nullptr);

  std::array<float, 4> output;
  result->readTo(output.data());

  for (int i = 0; i < 4; i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01f);
  }
}

TEST_P(TestDistanceNeighbors, executeMaximumDistanceTouching)
{
  // Same label image as mean test. Each label has only one touching neighbor at distance 3,
  // so maximum equals mean: [0, 3, 3, 3]
  // clang-format off
  std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2
  };
  // clang-format on
  std::array<float, 4> valid = { 0.0f, 3.0f, 3.0f, 3.0f };

  auto gpu_labels = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(input.data());

  auto centroids = cle::tier4::centroids_of_labels_func(device, gpu_labels, nullptr, false);
  auto distance_mat = cle::tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto touch_mat = cle::tier3::generate_touch_matrix_func(device, gpu_labels, nullptr);
  auto result = cle::tier1::maximum_distance_touching_neighbors_func(device, distance_mat, touch_mat, nullptr);

  std::array<float, 4> output;
  result->readTo(output.data());

  for (int i = 0; i < 4; i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01f);
  }
}

TEST_P(TestDistanceNeighbors, executeMeanDistanceNNearestNeighbors)
{
  // Same label image: label1=(1,1), label2=(4,4), label3=(4,1)
  // d(1,2)=sqrt(18)~4.24, d(1,3)=3, d(2,3)=3
  // With n=1 (1 nearest neighbor):
  //   label 1: nearest is label 3 at d=3  -> mean = 3
  //   label 2: nearest is label 3 at d=3  -> mean = 3
  //   label 3: nearest is label 1 or 2 at d=3 -> mean = 3
  // clang-format off
  std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2
  };
  // clang-format on
  std::array<float, 4> valid = { 0.0f, 3.0f, 3.0f, 3.0f };

  auto gpu_labels = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(input.data());

  auto centroids = cle::tier4::centroids_of_labels_func(device, gpu_labels, nullptr, false);
  auto distance_mat = cle::tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto result = cle::tier1::mean_distance_n_nearest_neighbors_func(device, distance_mat, nullptr, 1);

  std::array<float, 4> output;
  result->readTo(output.data());

  for (int i = 0; i < 4; i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01f);
  }
}

TEST_P(TestDistanceNeighbors, executeMaximumDistanceNNearestNeighbors)
{
  // Same label image: label1=(1,1), label2=(4,4), label3=(4,1)
  // d(1,2)=sqrt(18)~4.24, d(1,3)=3, d(2,3)=3
  // With n=2 (2 nearest neighbors):
  //   label 1: nearest 2 are [3, sqrt(18)] -> max = sqrt(18) ~ 4.24
  //   label 2: nearest 2 are [3, sqrt(18)] -> max = sqrt(18) ~ 4.24
  //   label 3: nearest 2 are [3, 3]        -> max = 3
  // clang-format off
  std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2
  };
  // clang-format on
  const float          sqrt18 = std::sqrt(18.0f);
  std::array<float, 4> valid = { 0.0f, sqrt18, sqrt18, 3.0f };

  auto gpu_labels = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(input.data());

  auto centroids = cle::tier4::centroids_of_labels_func(device, gpu_labels, nullptr, false);
  auto distance_mat = cle::tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto result = cle::tier1::maximum_distance_n_nearest_neighbors_func(device, distance_mat, nullptr, 2);

  std::array<float, 4> output;
  result->readTo(output.data());

  for (int i = 0; i < 4; i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01f);
  }
}

TEST_P(TestDistanceNeighbors, executeMeanDistanceNFarthestNeighbors)
{
  // Same label image: label1=(1,1), label2=(4,4), label3=(4,1)
  // d(1,2)=sqrt(18)~4.24, d(1,3)=3, d(2,3)=3
  // With n=1 (1 farthest neighbor):
  //   label 1: farthest is label 2 at sqrt(18) -> mean = sqrt(18) ~ 4.24
  //   label 2: farthest is label 1 at sqrt(18) -> mean = sqrt(18) ~ 4.24
  //   label 3: farthest is label 1 or 2 at d=3 -> mean = 3
  // clang-format off
  std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    1, 1, 1, 3, 3, 3,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2,
    0, 0, 0, 2, 2, 2
  };
  // clang-format on
  const float          sqrt18 = std::sqrt(18.0f);
  std::array<float, 4> valid = { 0.0f, sqrt18, sqrt18, 3.0f };

  auto gpu_labels = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(input.data());

  auto centroids = cle::tier4::centroids_of_labels_func(device, gpu_labels, nullptr, false);
  auto distance_mat = cle::tier1::generate_distance_matrix_func(device, centroids, centroids, nullptr);
  auto result = cle::tier1::mean_distance_n_farthest_neighbors_func(device, distance_mat, nullptr, 1);

  std::array<float, 4> output;
  result->readTo(output.data());

  for (int i = 0; i < 4; i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.01f);
  }
}


INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDistanceNeighbors, ::testing::ValuesIn(getParameters()));
