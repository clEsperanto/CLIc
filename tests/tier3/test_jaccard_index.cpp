#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestJaccardIndex : public ::testing::TestWithParam<std::string>
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

TEST_P(TestJaccardIndex, execute2D)
{
  std::array<float, 5 * 2 * 1> input1 = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0 };
  std::array<float, 5 * 2 * 1> input2 = { 0, 1, 1, 0, 0, 0, 1, 1, 0, 0 };

  auto gpu_input1 = cle::Array::create(5, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto output = cle::tier3::jaccard_index_func(device, gpu_input1, gpu_input2);

  EXPECT_EQ(output, 0.5);
}

TEST_P(TestJaccardIndex, execute3D)
{
  std::array<float, 3 * 2 * 2> input1 = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0 };
  std::array<float, 3 * 2 * 2> input2 = { 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0 };

  auto gpu_input1 = cle::Array::create(3, 2, 2, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  auto gpu_input2 = cle::Array::create(gpu_input1);
  gpu_input1->writeFrom(input1.data());
  gpu_input2->writeFrom(input2.data());

  auto output = cle::tier3::jaccard_index_func(device, gpu_input1, gpu_input2);

  EXPECT_EQ(output, 0.5);
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestJaccardIndex, ::testing::ValuesIn(getParameters()));
