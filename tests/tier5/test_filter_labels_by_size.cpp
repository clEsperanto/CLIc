#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>
#include "test_utils.hpp"

class TestFilterLabelsBySize : public ::testing::TestWithParam<std::string>
{
protected:
  std::string backend;
  cle::Device::Pointer device;
  std::array<uint32_t, 6 * 5 * 1> input = { 1, 1, 2, 0, 3, 3, 1, 1, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 5, 6, 6, 6, 4, 4, 5, 6, 6, 6 };

  std::array<uint32_t, 6 * 5 * 1> valid = { 1, 1, 0, 0, 2, 2, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0 };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestFilterLabelsBySize, execute2d)
{
  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::filter_label_by_size_func(device, gpu_input, nullptr, 4, 5);

  std::vector<uint32_t> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestFilterLabelsBySize, ::testing::ValuesIn(getParameters()));
