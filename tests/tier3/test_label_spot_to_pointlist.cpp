#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestLabelSpotToPointList : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1>    input = { 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 4 };
  std::array<uint32_t, 4 * 2 * 1> valid = { 1, 2, 3, 4, 1, 3, 1, 4 };
  std::array<uint32_t, 4 * 2 * 1> output;
};

TEST_P(TestLabelSpotToPointList, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier3::labelled_spots_to_pointlist_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelSpotToPointList, ::testing::ValuesIn(getParameters()));
