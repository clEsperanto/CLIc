#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestCentroidsOfLabels : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 5 * 1> input = { 1, 1, 2, 0, 3, 3, 1, 1, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 5, 6, 6, 6, 4, 4, 5, 6, 6, 6 };

  std::array<float, 7 * 3 * 1> valid = { 2.625, 0.5, 2, 4.5, 0.5, 2, 4, 1.625, 0.5, 0.5, 0.5, 3.5, 3.5, 3.5, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestCentroidsOfLabels, execute2d)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier4::centroids_of_labels_func(device, gpu_input, nullptr, true);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestCentroidsOfLabels, allBackgroundWithoutBackgroundCentroid)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<uint32_t, 6 * 5 * 1> all_background{};
  auto                            gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(all_background.data());

  auto gpu_output = cle::tier4::centroids_of_labels_func(device, gpu_input, nullptr, false);

  EXPECT_EQ(gpu_output->width(), 1);
  EXPECT_EQ(gpu_output->height(), 3);
  std::array<float, 3> output{};
  gpu_output->readTo(output.data());
  EXPECT_EQ(output, (std::array<float, 3>{ 0, 0, 0 }));
}

TEST_P(TestCentroidsOfLabels, rejectsNegativeOnlyLabels)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  std::array<int32_t, 6 * 5 * 1> negative_labels;
  negative_labels.fill(-1);
  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::INT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(negative_labels.data());

  EXPECT_THROW(cle::tier4::centroids_of_labels_func(device, gpu_input, nullptr, false), std::runtime_error);
}

TEST_P(TestCentroidsOfLabels, rejectsOutputWithDepth)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::Array::create(7, 3, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);

  EXPECT_THROW(cle::tier4::centroids_of_labels_func(device, gpu_input, gpu_output, true), std::runtime_error);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCentroidsOfLabels, ::testing::ValuesIn(getParameters()));
