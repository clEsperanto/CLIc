#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestSmoothConnectedLabels : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 9 * 7 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 0, 0, 0, 0, 1,
                                                  1, 2, 2, 2, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 0, 0, 0, 1, 1, 2, 2,
                                                  2, 0, 0, 0, 0, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  const std::array<uint32_t, 9 * 7 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 0, 0, 0, 1, 1,
                                                  1, 2, 2, 2, 2, 0, 0, 1, 1, 1, 2, 2, 2, 2, 0, 0, 1, 1, 1, 2, 2,
                                                  2, 2, 0, 0, 0, 1, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<uint32_t, 9 * 7 * 1> output;
};

TEST_P(TestSmoothConnectedLabels, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(9, 7, 1, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier8::smooth_connected_labels_func(device, gpu_input, nullptr, 1);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSmoothConnectedLabels, ::testing::ValuesIn(getParameters()));
