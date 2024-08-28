#include "cle.hpp"

#include "tier3.hpp"

#include <array>
#include <cmath>

#include <gtest/gtest.h>

class TestStatisticsOfLabelledPixels : public ::testing::TestWithParam<std::string>
{
protected:
  void
  print_stats_dict(std::unordered_map<std::string, std::vector<float>> region_props)
  {
    std::vector<std::string> keys = { "label",
                                      "original_label",
                                      "bbox_min_x",
                                      "bbox_min_y",
                                      "bbox_min_z",
                                      "bbox_max_x",
                                      "bbox_max_y",
                                      "bbox_max_z",
                                      "bbox_width",
                                      "bbox_height",
                                      "bbox_depth",
                                      "min_intensity",
                                      "max_intensity",
                                      "sum_intensity",
                                      "area",
                                      "mean_intensity",
                                      "sum_intensity_times_x",
                                      "mass_center_x",
                                      "sum_intensity_times_y",
                                      "mass_center_y",
                                      "sum_intensity_times_z",
                                      "mass_center_z",
                                      "sum_x",
                                      "centroid_x",
                                      "sum_y",
                                      "centroid_y",
                                      "sum_z",
                                      "centroid_z",
                                      "sum_distance_to_centroid",
                                      "mean_distance_to_centroid",
                                      "sum_distance_to_mass_center",
                                      "mean_distance_to_mass_center",
                                      "standard_deviation_intensity",
                                      "max_distance_to_centroid",
                                      "max_distance_to_mass_center",
                                      "mean_max_distance_to_centroid_ratio",
                                      "mean_max_distance_to_mass_center_ratio" };
    for (const auto & key : keys)
    {
      std::cout << key << ": { ";
      auto it = region_props.find(key);
      if (it == region_props.end())
      {
        std::cout << "not found }" << std::endl;
        continue;
      }
      for (const auto & value : it->second)
      {
        std::cout << std::fixed << std::setprecision(2) << value << " ";
      }
      std::cout << "} " << std::endl;
    }
  }
};

TEST_P(TestStatisticsOfLabelledPixels, execute2D)
{
  std::array<float, 3 * 3 * 1> intensity = { 0, 1, 2, 1, 2, 3, 2, 3, 4 };
  std::array<float, 3 * 3 * 1> labels = { 1, 1, 2, 1, 2, 2, 3, 3, 3 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_intensity = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());
  auto gpu_labels = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(labels.data());

  // passing labels also as intensity image to have a simpler test
  auto region_props = cle::tier3::statistics_of_labelled_pixels_func(device, gpu_labels, gpu_intensity, false);

  // Test bounding box min/max (x, y, z)
  std::vector<float> expected_bbox_min_x = { 0, 1, 0 };
  ASSERT_EQ(region_props["bbox_min_x"], expected_bbox_min_x);
  std::vector<float> expected_bbox_max_x = { 1, 2, 2 };
  ASSERT_EQ(region_props["bbox_max_x"], expected_bbox_max_x);
  std::vector<float> expected_bbox_min_y = { 0, 0, 2 };
  ASSERT_EQ(region_props["bbox_min_y"], expected_bbox_min_y);
  std::vector<float> expected_bbox_max_y = { 1, 1, 2 };
  ASSERT_EQ(region_props["bbox_max_y"], expected_bbox_max_y);
  std::vector<float> expected_bbox_min_z = { 0, 0, 0 };
  ASSERT_EQ(region_props["bbox_min_z"], expected_bbox_min_z);
  std::vector<float> expected_bbox_max_z = { 0, 0, 0 };
  ASSERT_EQ(region_props["bbox_max_z"], expected_bbox_max_z);
  // Test bounding box width/height/depth
  std::vector<float> expected_bbox_width = { 2, 2, 3 };
  ASSERT_EQ(region_props["bbox_width"], expected_bbox_width);
  std::vector<float> expected_bbox_height = { 2, 2, 1 };
  ASSERT_EQ(region_props["bbox_height"], expected_bbox_height);
  std::vector<float> expected_bbox_depth = { 1, 1, 1 };
  ASSERT_EQ(region_props["bbox_depth"], expected_bbox_depth);

  // { 1, 1, 2,
  //   1, 2, 2,
  //   3, 3, 3 };

  // Test centroids
  std::vector<float> expected_centroid_x = { 0.33333334, 1.6666667, 1.0 };
  std::vector<float> expected_centroid_y = { 0.33333334, 0.6666667, 2.0 };
  std::vector<float> expected_centroid_z = { 0, 0, 0 };
  for (int i = 0; i < region_props["centroid_x"].size(); i++)
  {
    ASSERT_NEAR(region_props["centroid_x"][i], expected_centroid_x[i], 0.001);
    ASSERT_NEAR(region_props["centroid_y"][i], expected_centroid_y[i], 0.001);
    ASSERT_NEAR(region_props["centroid_z"][i], expected_centroid_z[i], 0.001);
  }
  // Test intensities
  std::vector<float> expected_min_intensity = { 0.0, 2.0, 2.0 };
  ASSERT_EQ(region_props["min_intensity"], expected_min_intensity);
  std::vector<float> expected_max_intensity = { 1.0, 3.0, 4.0 };
  ASSERT_EQ(region_props["max_intensity"], expected_max_intensity);
  std::vector<float> expected_mean_intensity = { 0.6666667, 2.3333335, 3.0 };
  std::vector<float> expected_std_intensity = { 0.47140455, 0.47140455, 0.81649655 };
  for (size_t i = 0; i < expected_mean_intensity.size(); i++)
  {
    ASSERT_NEAR(region_props["mean_intensity"][i], expected_mean_intensity[i], 0.001);
    ASSERT_NEAR(region_props["standard_deviation_intensity"][i], expected_std_intensity[i], 0.001);
  }
}

TEST_P(TestStatisticsOfLabelledPixels, execute3D)
{
  std::array<float, 3 * 2 * 3> intensity = { 0, 1, 2, 0, 1, 2, 1, 2, 3, 1, 2, 3, 2, 3, 4, 2, 3, 4 };
  std::array<float, 3 * 2 * 3> labels = { 1, 1, 2, 1, 1, 2, 1, 2, 2, 1, 2, 2, 3, 3, 3, 3, 3, 3 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_intensity = cle::Array::create(3, 2, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());
  auto gpu_labels = cle::Array::create(3, 2, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(labels.data());

  auto region_props = cle::tier3::statistics_of_labelled_pixels_func(device, gpu_labels, gpu_intensity, false);

  // Test bounding box min/max (x, y, z)
  std::vector<float> expected_bbox_min_x = { 0, 1, 0 };
  ASSERT_EQ(region_props["bbox_min_x"], expected_bbox_min_x);
  std::vector<float> expected_bbox_min_y = { 0, 0, 0 };
  ASSERT_EQ(region_props["bbox_min_y"], expected_bbox_min_y);
  std::vector<float> expected_bbox_min_z = { 0, 0, 2 };
  ASSERT_EQ(region_props["bbox_min_z"], expected_bbox_min_z);

  std::vector<float> expected_bbox_max_x = { 1, 2, 2 };
  ASSERT_EQ(region_props["bbox_max_x"], expected_bbox_max_x);
  std::vector<float> expected_bbox_max_y = { 1, 1, 1 };
  ASSERT_EQ(region_props["bbox_max_y"], expected_bbox_max_y);
  std::vector<float> expected_bbox_max_z = { 1, 1, 2 };
  ASSERT_EQ(region_props["bbox_max_z"], expected_bbox_max_z);

  // Test bounding box width/height/depth
  std::vector<float> expected_bbox_width = { 2, 2, 3 };
  ASSERT_EQ(region_props["bbox_width"], expected_bbox_width);
  std::vector<float> expected_bbox_height = { 2, 2, 2 };
  ASSERT_EQ(region_props["bbox_height"], expected_bbox_height);
  std::vector<float> expected_bbox_depth = { 2, 2, 1 };
  ASSERT_EQ(region_props["bbox_depth"], expected_bbox_depth);

  // Test intensities
  std::vector<float> expected_min_intensity = { 0, 2, 2 };
  ASSERT_EQ(region_props["min_intensity"], expected_min_intensity);
  std::vector<float> expected_max_intensity = { 1, 3, 4 };
  ASSERT_EQ(region_props["max_intensity"], expected_max_intensity);
  std::vector<float> expected_sum_intensity = { 4, 14, 18 };
  ASSERT_EQ(region_props["sum_intensity"], expected_sum_intensity);

  std::vector<float> expected_mean_intensity = { 0.6666667, 2.3333333, 3.0 };
  std::vector<float> expected_std_intensity = { 0.4714045, 0.47140455, 0.81649655 };
  for (size_t i = 0; i < expected_mean_intensity.size(); i++)
  {
    ASSERT_NEAR(region_props["mean_intensity"][i], expected_mean_intensity[i], 0.001);
    ASSERT_NEAR(region_props["standard_deviation_intensity"][i], expected_std_intensity[i], 0.001);
  }

  // Test centroids
  std::vector<float> expected_centroid_x = { 0.33333334, 1.6666667, 1.0 };
  std::vector<float> expected_centroid_y = { 0.5, 0.5, 0.5 };
  std::vector<float> expected_centroid_z = { 0.33333334, 0.6666667, 2.0 };
  for (int i = 0; i < region_props["centroid_x"].size(); i++)
  {
    ASSERT_NEAR(region_props["centroid_x"][i], expected_centroid_x[i], 0.001);
    ASSERT_NEAR(region_props["centroid_y"][i], expected_centroid_y[i], 0.001);
    ASSERT_NEAR(region_props["centroid_z"][i], expected_centroid_z[i], 0.001);
  }

  std::vector<float> expected_mean_distance_to_centroid = { 0.8274131, 0.8274131, 0.9120226 };
  std::vector<float> expected_mean_distance_to_mass_center = { 0.8660254, 0.8259635, 0.93077606 };
  std::vector<float> expected_max_distance_to_centroid = { 0.89752746, 0.8975275, 1.1180339 };
  std::vector<float> expected_max_distance_to_mass_center = { 0.8660254, 0.9175167, 1.3205405 };
  std::vector<float> expected_mean_max_distance_to_centroid_ratio = { 1.0847392, 1.0847393, 1.225884 };
  std::vector<float> expected_mean_max_distance_to_mass_center_ratio = { 1.0, 1.1108441, 1.4187522 };
  for (size_t i = 0; i < expected_mean_distance_to_centroid.size(); i++)
  {
    ASSERT_NEAR(region_props["mean_distance_to_centroid"][i], expected_mean_distance_to_centroid[i], 0.001);
    ASSERT_NEAR(region_props["mean_distance_to_mass_center"][i], expected_mean_distance_to_mass_center[i], 0.001);
    ASSERT_NEAR(region_props["max_distance_to_centroid"][i], expected_max_distance_to_centroid[i], 0.001);
    ASSERT_NEAR(region_props["max_distance_to_mass_center"][i], expected_max_distance_to_mass_center[i], 0.001);
    ASSERT_NEAR(
      region_props["mean_max_distance_to_centroid_ratio"][i], expected_mean_max_distance_to_centroid_ratio[i], 0.001);
    ASSERT_NEAR(region_props["mean_max_distance_to_mass_center_ratio"][i],
                expected_mean_max_distance_to_mass_center_ratio[i],
                0.001);
  }
}

TEST_P(TestStatisticsOfLabelledPixels, shape)
{
  std::array<float, 5 * 5 * 1> labels = { 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_labels = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(labels.data());

  // passing labels also as intensity image to have a simpler test
  auto region_props = cle::tier3::statistics_of_labelled_pixels_func(device, gpu_labels, gpu_labels, false);

  // Test bounding box min/max (x, y, z)
  std::vector<float> expected_bbox_min_x = { 0 };
  ASSERT_EQ(region_props["bbox_min_x"], expected_bbox_min_x);
  std::vector<float> expected_bbox_min_y = { 0 };
  ASSERT_EQ(region_props["bbox_min_y"], expected_bbox_min_y);
  std::vector<float> expected_bbox_min_z = { 0 };
  ASSERT_EQ(region_props["bbox_min_z"], expected_bbox_min_z);
  std::vector<float> expected_bbox_max_x = { 4 };
  ASSERT_EQ(region_props["bbox_max_x"], expected_bbox_max_x);
  std::vector<float> expected_bbox_max_y = { 4 };
  ASSERT_EQ(region_props["bbox_max_y"], expected_bbox_max_y);
  std::vector<float> expected_bbox_max_z = { 0 };
  ASSERT_EQ(region_props["bbox_max_z"], expected_bbox_max_z);

  // Test bounding box width/height/depth
  std::vector<float> expected_bbox_width = { 5 };
  ASSERT_EQ(region_props["bbox_width"], expected_bbox_width);
  std::vector<float> expected_bbox_height = { 5 };
  ASSERT_EQ(region_props["bbox_height"], expected_bbox_height);
  std::vector<float> expected_bbox_depth = { 1 };
  ASSERT_EQ(region_props["bbox_depth"], expected_bbox_depth);

  // Test centroids
  std::vector<float> expected_centroid_x = { 2 };
  std::vector<float> expected_centroid_y = { 2 };
  std::vector<float> expected_centroid_z = { 0 };
  for (int i = 0; i < region_props["centroid_x"].size(); i++)
  {
    ASSERT_NEAR(region_props["centroid_x"][i], expected_centroid_x[i], 0.001);
    ASSERT_NEAR(region_props["centroid_y"][i], expected_centroid_y[i], 0.001);
    ASSERT_NEAR(region_props["centroid_z"][i], expected_centroid_z[i], 0.001);
  }

  std::vector<float> expected_mean_distance_to_centroid = { 1.3582196 };
  ASSERT_NEAR(region_props["mean_distance_to_centroid"][0], expected_mean_distance_to_centroid[0], 0.001);
  std::vector<float> expected_mean_distance_to_mass_center = { 1.3582196 };
  ASSERT_NEAR(region_props["mean_distance_to_mass_center"][0], expected_mean_distance_to_mass_center[0], 0.001);
  std::vector<float> expected_max_distance_to_centroid = { 2.0 };
  ASSERT_NEAR(region_props["max_distance_to_centroid"][0], expected_max_distance_to_centroid[0], 0.001);
  std::vector<float> expected_max_distance_to_mass_center = { 2.0 };
  ASSERT_NEAR(region_props["max_distance_to_mass_center"][0], expected_max_distance_to_mass_center[0], 0.001);
  std::vector<float> expected_mean_max_distance_to_centroid_ratio = { 1.472516 };
  ASSERT_NEAR(
    region_props["mean_max_distance_to_centroid_ratio"][0], expected_mean_max_distance_to_centroid_ratio[0], 0.001);
  std::vector<float> expected_mean_max_distance_to_mass_center_ratio = { 1.472516 };
  ASSERT_NEAR(region_props["mean_max_distance_to_mass_center_ratio"][0],
              expected_mean_max_distance_to_mass_center_ratio[0],
              0.001);
}


TEST_P(TestStatisticsOfLabelledPixels, standard_deviation)
{
  std::array<float, 3 * 3 * 1> image = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  std::array<float, 3 * 3 * 1> labels = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_image = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_image->writeFrom(image.data());
  auto gpu_labels = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->writeFrom(labels.data());

  auto region_props = cle::tier3::statistics_of_labelled_pixels_func(device, gpu_labels, gpu_image, false);

  // print_stats_dict(region_props);

  auto it = region_props.find("standard_deviation_intensity");
  auto std_dev = it->second;

  EXPECT_NEAR(std_dev[0], 2.5819888, 0.001);
}

std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStatisticsOfLabelledPixels, ::testing::ValuesIn(getParameters()));
