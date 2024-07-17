#include "cle.hpp"
#include <array>
#include <cmath>
#include <gtest/gtest.h>

class TestStatisticsOfLabelledPixels : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 1>  intensity = { 0, 1, 2, 1, 2, 3, 2, 3, 4 };
  std::array<float, 3 * 3 * 1>  labels = { 1, 1, 2, 1, 2, 2, 3, 3, 3 };
  std::array<float, 36 * 3 * 1> output;
};

TEST_P(TestStatisticsOfLabelledPixels, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_intensity = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->write(intensity.data());
  auto gpu_labels = cle::Array::create(3, 3, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_labels->write(labels.data());

  // { 1, 1, 2,
  //   1, 2, 2,
  //   3, 3, 3 };

  // passing labels also as intensity image to have a simpler test
  auto region_props = cle::statistics_of_labelled_pixels(device, gpu_labels, gpu_labels, nullptr);

  // Test bounding box
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

  std::vector<float> expected_bbox_width = { 2, 2, 3 };
  ASSERT_EQ(region_props["bbox_width"], expected_bbox_width);
  std::vector<float> expected_bbox_height = { 2, 2, 1 };
  ASSERT_EQ(region_props["bbox_height"], expected_bbox_height);
  std::vector<float> expected_bbox_depth = { 1, 1, 1 };
  ASSERT_EQ(region_props["bbox_depth"], expected_bbox_depth);


  // Test intensities
  std::vector<float> expected_min_intensity = { 1, 2, 3 };
  ASSERT_EQ(region_props["min_intensity"], expected_min_intensity);
  std::vector<float> expected_max_intensity = { 1, 2, 3 };
  ASSERT_EQ(region_props["max_intensity"], expected_max_intensity);
  std::vector<float> expected_mean_intensity = { 1, 2, 3 };
  ASSERT_EQ(region_props["mean_intensity"], expected_mean_intensity);
  std::vector<float> expected_std_intensity = { 0, 0, 0 };
  ASSERT_EQ(region_props["standard_deviation_intensity"], expected_std_intensity);


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

class TestStatisticsOfLabelledPixels3D : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 3>  intensity = { 0, 1, 2, 1, 2, 3, 2, 3, 4 };
  std::array<float, 3 * 3 * 3>  labels = { 1, 1, 2, 1, 2, 2, 3, 3, 3 };
  std::array<float, 36 * 3 * 1> reference = {
    // IDENTIFIER(0)
    1,
    2,
    3,
    // BOUNDING_BOX_X(1)
    0,
    1,
    0,
    // BOUNDING_BOX_Y(2)
    0,
    0,
    0,
    // ... [rest of the reference data]
  };
  std::array<float, 36 * 3 * 1> output;
};

TEST_P(TestStatisticsOfLabelledPixels3D, execute)
{
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

TEST(TestStandardDeviation, execute)
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
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStatisticsOfLabelledPixels3D, ::testing::ValuesIn(getParameters()));
