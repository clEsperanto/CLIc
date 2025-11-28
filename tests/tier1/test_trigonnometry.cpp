#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestTrigonometry : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 3 * 5 * 3> output;
  std::array<float, 3 * 5 * 3> input;
  std::array<float, 3 * 5 * 3> valid;

  virtual void
  SetUp()
  {
    std::fill(input.begin(), input.end(), static_cast<float>(1));
  }
};

TEST_P(TestTrigonometry, cos)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(0.5403086));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::cos_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}


TEST_P(TestTrigonometry, sin)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(0.84147096));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::sin_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}


TEST_P(TestTrigonometry, tan)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(1.55740772));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::tan_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}


TEST_P(TestTrigonometry, asin)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(1.57079633));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::asin_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}


TEST_P(TestTrigonometry, acos)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(0.0f));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::acos_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}


TEST_P(TestTrigonometry, tanh)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(0.76159416));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::tanh_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}


TEST_P(TestTrigonometry, cosh)
{
  std::fill(valid.begin(), valid.end(), static_cast<float>(1.54308063));

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(3, 5, 3, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());
  auto gpu_output = cle::tier1::cosh_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_NEAR(output[i], valid[i], 1e-5);
  }
}

/*
 * The inverse hyperbolic functions `asinh` and `atanh` are not currently
 * implemented in `cle::tier1` in this branch. If/when they are added,
 * re-enable or implement tests similar to the above patterns.
 */

// todo : test for tan, acos, asin, tanh, atanh, cosh, asinh

std::vector<std::string>
getParameters()
{
  std::vector<std::string> parameters;
#if USE_OPENCL
  parameters.push_back("opencl");
#endif
#if USE_CUDA
  parameters.push_back("cuda");
#endif
  return parameters;
}

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestTrigonometry, ::testing::ValuesIn(getParameters()));
