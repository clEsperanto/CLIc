#include "cle.hpp"

#include "test_utils.hpp"
#include <array>
#include <gtest/gtest.h>

class TestAffineTransform : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestAffineTransform, affineTransform)
{
  const std::array<float, 5 * 5 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const std::array<float, 5 * 5 * 1> valid = { 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  std::array<float, 5 * 5 * 1>       output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  std::vector<float> matrix = { 1, 0, -1, 0, 1, -1, 0, 0, 1 };
  auto               gpu_output = cle::tier7::affine_transform_func(device, gpu_input, nullptr, &matrix, false, false);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}


TEST_P(TestAffineTransform, affineTransformInterpolate)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  if (!device->supportImage())
  {
    GTEST_SKIP() << "Device does not support image objects.";
  }

  const std::array<float, 5 * 5 * 1>   input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  const std::array<float, 10 * 10 * 1> valid = {
    0, 0, 0, 0,      0,      0,      0,      0, 0, 0, 0, 0, 0, 0,      0,      0,      0,      0, 0, 0,
    0, 0, 0, 0,      0,      0,      0,      0, 0, 0, 0, 0, 0, 0.0625, 0.1875, 0.1875, 0.0625, 0, 0, 0,
    0, 0, 0, 0.1875, 0.5625, 0.5625, 0.1875, 0, 0, 0, 0, 0, 0, 0.1875, 0.5625, 0.5625, 0.1875, 0, 0, 0,
    0, 0, 0, 0.0625, 0.1875, 0.1875, 0.0625, 0, 0, 0, 0, 0, 0, 0,      0,      0,      0,      0, 0, 0,
    0, 0, 0, 0,      0,      0,      0,      0, 0, 0, 0, 0, 0, 0,      0,      0,      0,      0, 0, 0,
  };
  std::array<float, 10 * 10 * 1> output;

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  std::vector<float> matrix = { 2, 0, 0, 0, 2, 0, 0, 0, 1 };
  auto               gpu_output = cle::tier7::affine_transform_func(device, gpu_input, nullptr, &matrix, true, true);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
  }
}
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestAffineTransform, ::testing::ValuesIn(getParameters()));
