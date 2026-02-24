#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestMergeTouchingLabels : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 5 * 3 * 1> input = {
    1, 1, 0, 0, 0, 0, 2, 2, 0, 3, 0, 0, 2, 0, 3,
  };
  const std::array<uint32_t, 5 * 3 * 1> valid = {
    1, 1, 0, 0, 0, 0, 1, 1, 0, 2, 0, 0, 1, 0, 2,
  };
  std::array<uint32_t, 5 * 3 * 1> output;
};

TEST_P(TestMergeTouchingLabels, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 3, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::merge_touching_labels_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMergeTouchingLabels, ::testing::ValuesIn(getParameters()));
