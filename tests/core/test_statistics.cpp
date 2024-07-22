#include "cle.hpp"

#include "statistics.hpp"

#include <array>
#include <cmath>

#include <gtest/gtest.h>

class TestStatisticsOfLabelledPixels : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestStatisticsOfLabelledPixels, simple)
{

  std::array<float, 3 * 3 * 1> intensity = { 0, 1, 2, 1, 2, 3, 2, 3, 4 };
  std::array<float, 3 * 3 * 1> labels = { 1, 1, 2, 1, 2, 2, 3, 3, 3 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_intensity = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->write(intensity.data());
  auto gpu_labels = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->write(labels.data());

  // passing labels also as intensity image to have a simpler test
  auto region_props = cle::statistics_of_labelled_pixels(device, gpu_labels, gpu_labels, nullptr);

  // Print out all entries in region_props
  for (const auto & entry : region_props)
  {
    std::cout << entry.first << ": ";
    for (const auto & value : entry.second)
    {
      std::cout << std::fixed << std::setprecision(2) << value << " ";
    }
    std::cout << std::endl;
  }

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
  std::vector<float> expected_centroid_x = { 0.333, 1.666, 1 };
  std::vector<float> expected_centroid_y = { 0.333, 0.666, 2 };
  std::vector<float> expected_centroid_z = { 0, 0, 0 };
  for (int i = 0; i < region_props["centroid_x"].size(); i++)
  {
    ASSERT_NEAR(region_props["centroid_x"][i], expected_centroid_x[i], 0.001);
    ASSERT_NEAR(region_props["centroid_y"][i], expected_centroid_y[i], 0.001);
    ASSERT_NEAR(region_props["centroid_z"][i], expected_centroid_z[i], 0.001);
  }
  // Test intensities
  std::vector<float> expected_min_intensity = { 1, 2, 3 };
  ASSERT_EQ(region_props["min_intensity"], expected_min_intensity);
  std::vector<float> expected_max_intensity = { 1, 2, 3 };
  ASSERT_EQ(region_props["max_intensity"], expected_max_intensity);
  std::vector<float> expected_mean_intensity = { 1, 2, 3 };
  ASSERT_EQ(region_props["mean_intensity"], expected_mean_intensity);
  std::vector<float> expected_std_intensity = { 0, 0, 0 };
  ASSERT_EQ(region_props["standard_deviation_intensity"], expected_std_intensity);
}

TEST_P(TestStatisticsOfLabelledPixels, shape)
{
  std::array<float, 5 * 5 * 1> labels = { 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_labels = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->write(labels.data());


  // passing labels also as intensity image to have a simpler test
  auto region_props = cle::statistics_of_labelled_pixels(device, gpu_labels, gpu_labels, nullptr);

  // Print out all entries in region_props
  for (const auto & entry : region_props)
  {
    std::cout << entry.first << ": ";
    for (const auto & value : entry.second)
    {
      std::cout << std::fixed << std::setprecision(2) << value << " ";
    }
    std::cout << std::endl;
  }

  // Test bounding box min/max (x, y, z)
  std::vector<float> expected_bbox_min_x = { 0 };
  ASSERT_EQ(region_props["bbox_min_x"], expected_bbox_min_x);
  std::vector<float> expected_bbox_max_x = { 4 };
  ASSERT_EQ(region_props["bbox_max_x"], expected_bbox_max_x);
  std::vector<float> expected_bbox_min_y = { 0 };
  ASSERT_EQ(region_props["bbox_min_y"], expected_bbox_min_y);
  std::vector<float> expected_bbox_max_y = { 4 };
  ASSERT_EQ(region_props["bbox_max_y"], expected_bbox_max_y);
  std::vector<float> expected_bbox_min_z = { 0 };
  ASSERT_EQ(region_props["bbox_min_z"], expected_bbox_min_z);
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

  std::vector<float> expected_mean_distance_to_centroid = { 1 };
  ASSERT_EQ(region_props["mean_distance_to_centroid"], expected_mean_distance_to_centroid);
  std::vector<float> expected_mean_distance_to_mass_center = { 1 };
  ASSERT_EQ(region_props["mean_distance_to_mass_center"], expected_mean_distance_to_mass_center);
  std::vector<float> expected_max_distance_to_centroid = { 1 };
  ASSERT_EQ(region_props["max_distance_to_centroid"], expected_max_distance_to_centroid);
  std::vector<float> expected_max_distance_to_mass_center = { 1 };
  ASSERT_EQ(region_props["max_distance_to_mass_center"], expected_max_distance_to_mass_center);
  std::vector<float> expected_mean_max_distance_to_centroid_ratio = { 1 };
  ASSERT_EQ(region_props["mean_max_distance_to_centroid_ratio"], expected_mean_max_distance_to_centroid_ratio);
  std::vector<float> expected_mean_max_distance_to_mass_center_ratio = { 1 };
  ASSERT_EQ(region_props["mean_max_distance_to_mass_center_ratio"], expected_mean_max_distance_to_mass_center_ratio);
}


TEST_P(TestStatisticsOfLabelledPixels, dim3)
{
  std::array<float, 3 * 3 * 3> intensity = { 0, 1, 2, 1, 2, 3, 2, 3, 4 };
  std::array<float, 3 * 3 * 3> labels = { 1, 1, 2, 1, 2, 2, 3, 3, 3 };
  // std::array<float, 36 * 3 * 1> reference = {
  //     // IDENTIFIER(0)
  //     1,
  //     2,
  //     3,
  //     // BOUNDING_BOX_X(1)
  //     0,
  //     1,
  //     0,
  //     // BOUNDING_BOX_Y(2)
  //     0,
  //     0,
  //     0,
  //     // ... [rest of the reference data]
  //   };

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_intensity = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->write(intensity.data());
  auto gpu_labels = cle::Array::create(3, 3, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->write(labels.data());

  auto region_props = cle::statistics_of_labelled_pixels(device, gpu_intensity, gpu_labels, nullptr);

  // Test intensities
  std::vector<float> expected = { 1, 2, 3, 4 };
  ASSERT_EQ(region_props["min_intensity"], expected);
  ASSERT_EQ(region_props["max_intensity"], expected);
  ASSERT_EQ(region_props["mean_intensity"], expected);
  // std::vector<float> expected_std_intensity = {0, 0, 0, 0};
  // ASSERT_EQ(region_props["standard_deviation_intensity"], expected_std_intensity);


  // Assuming cle::push_regionprops is implemented
  /*
  auto result_image = cle::tier1::push_regionprops_func(device, result, 1, nullptr);

result_image->read(output.data());

  for (int i = 0; i < output.size(); i++)
  {
      EXPECT_NEAR(output[i], reference[i], 0.0001);
  }
  */
}

TEST_P(TestStatisticsOfLabelledPixels, standard_deviation)
{
  std::array<float, 3 * 3> image = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  std::array<float, 3 * 3> labels = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_image = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_image->write(image.data());
  auto gpu_labels = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->write(labels.data());

  auto result = cle::statistics_of_labelled_pixels(device, gpu_image, gpu_labels, nullptr);

  // find the value in result with key = "std_dev"
  auto it = result.find("standard_deviation_intensity");
  auto std_dev = it->second;

  EXPECT_NEAR(std_dev[0], 2.58, 0.1);
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
