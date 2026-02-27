#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestCombineLabels : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                     backend;
  cle::Device::Pointer            device;
  std::array<uint32_t, 6 * 2 * 1> input1 = { 1, 1, 0, 0, 3, 3, 1, 1, 0, 2, 3, 3 };
  std::array<uint32_t, 6 * 2 * 1> input2 = { 0, 1, 1, 2, 2, 0, 0, 1, 1, 2, 2, 0 };
  std::array<uint32_t, 6 * 2 * 1> valid = { 1, 3, 3, 4, 4, 2, 1, 3, 3, 4, 4, 2 };
  std::array<uint32_t, 6 * 2 * 1> output;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestCombineLabels, execute)
{
  auto gpu_input1 = cle::Array::create(6, 2, 1, 3, cle::dType::UINT32, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto gpu_output = cle::tier5::combine_labels_func(device, gpu_input1, gpu_input2, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestCombineLabels, ::testing::ValuesIn(getParameters()));
