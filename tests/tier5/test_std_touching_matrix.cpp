#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestStdTouchingMatrix : public ::testing::TestWithParam<std::string>
{
protected:
  std::string                           backend;
  cle::Device::Pointer                  device;
  const std::array<uint32_t, 6 * 6 * 1> input = {
    1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 2, 2, 2, 4, 4, 4, 2, 2, 2, 4, 4, 5, 2, 2, 2, 4, 5, 5,
  };
  const std::array<float, 6> valid = { 0.0f, 0.10000001f, 0.11111111f, 0.12698412f, 0.04444444f, 0.0f };
  std::array<float, 6>       output;

  void
  SetUp() override
  {
    backend = GetParam();
    cle::BackendManager::getInstance().setBackend(backend);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestStdTouchingMatrix, executeStandardDeviationPartialTouchingAreaMatrix)
{
  auto gpu_input = cle::Array::create(6, 6, 1, 2, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::standard_deviation_partial_touching_area_matrix_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 0.001f);
  }
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestStdTouchingMatrix, ::testing::ValuesIn(getParameters()));
