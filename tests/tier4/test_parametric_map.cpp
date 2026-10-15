#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestParametricMap : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 4 * 2> input = { 1, 1, 2, 0, 3, 4, 4, 0 };
  std::array<float, 4 * 2>    output;
};

TEST_P(TestParametricMap, shortProperty)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 2, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  // values for labels 1 and 2 only (measured without the background): labels 3 and 4 have no value and map to 0
  const cle::StatisticsMap properties = { { "area", { 10, 20 } } };
  auto                     gpu_output = cle::tier4::parametric_map_func(device, gpu_input, properties, nullptr, "AREA");

  const std::array<float, 4 * 2> valid = { 10, 10, 20, 0, 0, 0, 0, 0 };
  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}

TEST_P(TestParametricMap, unknownProperty)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(4, 2, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  EXPECT_THROW(cle::tier4::parametric_map_func(device, gpu_input, { { "area", { 1 } } }, nullptr, "volume"), std::runtime_error);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestParametricMap, ::testing::ValuesIn(getParameters()));
