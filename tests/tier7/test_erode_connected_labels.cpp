#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestErodeConnectedLabels : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 7 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 2, 0, 0, 1, 1, 2,
                                                  2, 2, 0, 0, 1, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 };

  const std::array<uint32_t, 7 * 5 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2,
                                                  2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  std::array<uint32_t, 7 * 5 * 1> output;
};

TEST_P(TestErodeConnectedLabels, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(7, 5, 1, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier7::erode_connected_labels_func(device, gpu_input, nullptr, 1);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestErodeConnectedLabels, ::testing::ValuesIn(getParameters()));
