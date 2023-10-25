
#include "cle.hpp"

#include <gtest/gtest.h>

class TestBackends : public ::testing::TestWithParam<std::string>
{};

TEST_P(TestBackends, manager)
{
  auto backend_list = cle::BackendManager::getInstance().getBackendsList();
  EXPECT_FALSE(backend_list.empty());

  auto ocl_flag = cle::BackendManager::getInstance().openCLEnabled();
  auto cuda_flag = cle::BackendManager::getInstance().cudaEnabled();
  EXPECT_TRUE(ocl_flag || cuda_flag);

  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);

  auto backend_type = cle::BackendManager::getInstance().getBackend().getType();
  EXPECT_TRUE(backend_type == cle::Backend::Type::CUDA || backend_type == cle::Backend::Type::OPENCL);
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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestBackends, ::testing::ValuesIn(getParameters()));
