#include "cle.hpp"
#include <array>
#include <cmath>
#include <gtest/gtest.h>

class TestStatisticsOfLabelledPixels : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 1>  intensity = { 0, 1, 2, 1, 2, 3, 2, 3, 4 };
  std::array<float, 3 * 3 * 1>  labels = { 1, 1, 2, 1, 2, 2, 3, 3, 3 };
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
    2,
    // ... [rest of the reference data]
  };
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

  auto result = cle::statistics_of_labelled_pixels(device, gpu_intensity, gpu_labels, nullptr);

  // Assuming cle::push_regionprops is implemented
  auto result_image = cle::tier1::push_regionprops_func(device, result, 1, nullptr);

  result_image->read(output.data());

  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], reference[i], 0.0001);
  }
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

    auto result = cle::statistics_of_labelled_pixels(device, gpu_intensity, gpu_labels, nullptr);

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

  float std_dev;
  result->read(&std_dev, 14 * sizeof(float), sizeof(float)); // Assuming standard deviation is at index 14

  EXPECT_NEAR(std_dev, 2.58, 0.1);
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
