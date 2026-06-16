#include "cle.hpp"

#include "test_utils.hpp"
#include <gtest/gtest.h>

class TestMakeIsotropic : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<float, 4 * 4 * 1> input = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
  std::string                     backend;
  cle::Device::Pointer            device;

  virtual void
  SetUp()
  {
    std::string param = GetParam();
    cle::BackendManager::getInstance().setBackend(param);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestMakeIsotropic, makeIsotropicWithExplicitTargetSpacing)
{
  auto gpu_input = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output =
      cle::tier8::make_isotropic_func(device, gpu_input, nullptr, 2.0, 1.0, 1.0, 1.0, true);

  EXPECT_EQ(gpu_output->width(), 8);
  EXPECT_EQ(gpu_output->height(), 4);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestMakeIsotropic, makeIsotropicWithDefaultTargetSpacing)
{
  auto gpu_input = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output =
      cle::tier8::make_isotropic_func(device, gpu_input, nullptr, 2.0, 1.0, 0.5, -1.0, true);

  EXPECT_EQ(gpu_output->width(), 16);
  EXPECT_EQ(gpu_output->height(), 8);
  EXPECT_EQ(gpu_output->depth(), 1);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMakeIsotropic, ::testing::ValuesIn(getParameters()));

class TestMakeAnisotropic : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<float, 4 * 4 * 1> input = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
  std::string                     backend;
  cle::Device::Pointer            device;

  virtual void
  SetUp()
  {
    std::string param = GetParam();
    cle::BackendManager::getInstance().setBackend(param);
    device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
    device->setWaitToFinish(true);
  }
};

TEST_P(TestMakeAnisotropic, makeAnisotropicWithExplicitTargetSpacing)
{
  auto gpu_input = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output =
      cle::tier8::make_anisotropic_func(device, gpu_input, nullptr, 1.0, 2.0, 1.0, 1.0, true);

  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 4);
  EXPECT_EQ(gpu_output->depth(), 1);
}

TEST_P(TestMakeAnisotropic, makeAnisotropicWithDefaultCurrentSpacing)
{
  auto gpu_input = cle::Array::create(4, 4, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output =
      cle::tier8::make_anisotropic_func(device, gpu_input, nullptr, -1.0, 2.0, 1.0, 1.0, true);

  EXPECT_EQ(gpu_output->width(), 2);
  EXPECT_EQ(gpu_output->height(), 4);
  EXPECT_EQ(gpu_output->depth(), 1);
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestMakeAnisotropic,
                         ::testing::ValuesIn(getParameters()));
