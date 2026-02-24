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
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCentroidsOfLabels, ::testing::ValuesIn(getParameters()));
