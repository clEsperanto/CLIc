#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestLabelBoundingBox : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 7 * 7 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
                                         0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestLabelBoundingBox, execute2d)
{

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(7, 7, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto output = cle::tier4::label_bounding_box_func(device, gpu_input, 2);

  std::vector<float> valid = { 4, 4, 0, 5, 5, 0 };
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestLabelBoundingBox, ::testing::ValuesIn(getParameters()));
