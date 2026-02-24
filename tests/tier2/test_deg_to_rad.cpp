#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestDegreeToRadiant : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                  backend;
  cle::Device::Pointer         device;
  std::array<float, 3 * 1 * 1> output;
  std::array<float, 3 * 1 * 1> input = { 180, 0, -90 };
  std::array<float, 3 * 1 * 1> valid = { M_PI, 0, -0.5 * M_PI };

  virtual void
  SetUp()
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestDegreeToRadiant, execute)
{
  auto gpu_input = cle::Array::create(3, 1, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier2::degrees_to_radians_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDegreeToRadiant, ::testing::ValuesIn(getParameters()));
