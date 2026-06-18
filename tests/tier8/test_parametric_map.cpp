#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestParametricMap : public ::testing::TestWithParam<std::string>
{
protected:
  std::string          backend;
  cle::Device::Pointer device;

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }


};

TEST_P(TestParametricMap, executeArea)
{
  std::array<uint32_t, 6 * 5 * 1> input = { 1, 1, 2, 0, 3, 3, 1, 1, 2, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 5, 6, 6, 6, 4, 4, 5, 6, 6, 6 };
  std::array<float, 6 * 5 * 1> valid = { 4, 4, 2, 0, 4, 4, 4, 4, 2, 0, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 2, 6, 6, 6, 4, 4, 2, 6, 6, 6 };

  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto props = cle::tier3::labels_statistics_func(device, gpu_input, nullptr, true);
  auto gpu_output = cle::tier4::parametric_map_func(device, gpu_input, props, nullptr, std::string("area"));

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}


TEST_P(TestParametricMap, executeTouching)
{
  std::array<uint32_t, 6 * 5 * 1> input = { 
    1, 1, 2, 0, 3, 3, 
    1, 1, 2, 0, 3, 3, 
    0, 0, 0, 0, 0, 0, 
    4, 4, 5, 6, 6, 6, 
    4, 4, 5, 6, 6, 6 };
  std::array<float, 6 * 5 * 1> valid = { 
    1, 1, 1, 0, 0, 0, 
    1, 1, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 
    1, 1, 2, 1, 1, 1, 
    1, 1, 2, 1, 1, 1 };

  auto gpu_input = cle::Array::create(6, 5, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto props = cle::tier7::labels_neighbors_statistics_func(device, gpu_input, {}, {}, {}, true);
  auto gpu_output = cle::tier4::parametric_map_func(device, gpu_input, props, nullptr, std::string("touching_neighbor_count"));

  std::vector<float> output(gpu_output->size());
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestParametricMap, ::testing::ValuesIn(getParameters()));
