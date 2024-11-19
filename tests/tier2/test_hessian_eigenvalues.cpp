#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestHessianEigenvalues : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 2 * 2 * 1> input_2d = { 1, -1, 1, -1 };
  std::array<float, 2 * 2 * 1> small_2d = { -2, 0, -2, 0 };
  std::array<float, 2 * 2 * 1> large_2d = { 0, 2, 0, 2 };

  // std::array<float, 3 * 3 * 2> input_3d = { 1, -1, 1, -1 };
  // std::array<float, 3 * 3 * 2> small_3d = { -2, 0, -2, 0 };
  // std::array<float, 3 * 3 * 2> middl_3d = { -2, 0, -2, 0 };
  // std::array<float, 3 * 3 * 2> large_3d = { 0, 2, 0, 2 };
};

TEST_P(TestHessianEigenvalues, execute2D)
{
  std::array<float, 2 * 2 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2d.data());

  auto gpu_output = cle::tier1::hessian_eigenvalues_func(device, gpu_input, nullptr, nullptr, nullptr);

  EXPECT_EQ(gpu_output.size(), 2);
  gpu_output[0]->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], small_2d[i]);
  }
  gpu_output[1]->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], large_2d[i]);
  }
}

TEST_P(TestHessianEigenvalues, executeLarge2D)
{
  std::array<float, 2 * 2 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2d.data());

  auto gpu_output = cle::tier2::large_hessian_eigenvalue_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], large_2d[i]);
  }
}

TEST_P(TestHessianEigenvalues, executeSmall2D)
{
  std::array<float, 2 * 2 * 1> output;

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(2, 2, 1, 3, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input_2d.data());

  auto gpu_output = cle::tier2::small_hessian_eigenvalue_func(device, gpu_input, nullptr);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], small_2d[i]);
  }
}

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestHessianEigenvalues, ::testing::ValuesIn(getParameters()));
