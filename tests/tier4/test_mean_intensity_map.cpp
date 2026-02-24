#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMeanIntensityMap : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 3 * 1> intensity = { 1, 1, 2, 4, 0, 0, 5, 3, 0 };

  std::array<uint32_t, 3 * 3 * 1> label = { 1, 1, 2, 1, 0, 0, 3, 3, 0 };

  std::array<float, 3 * 3 * 1> valid = { 2, 2, 2, 2, 0, 0, 4, 4, 0 };
};

TEST_P(TestMeanIntensityMap, execute)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_intensity = cle::Array::create(3, 3, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_intensity->writeFrom(intensity.data());
  auto gpu_label = cle::Array::create(3, 3, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_label->writeFrom(label.data());

  auto gpu_output = cle::tier4::mean_intensity_map_func(device, gpu_intensity, gpu_label, nullptr);

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMeanIntensityMap, ::testing::ValuesIn(getParameters()));
