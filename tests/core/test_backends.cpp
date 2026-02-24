
#include "cle.hpp"

#include <gtest/gtest.h>
#include "test_utils.hpp"

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
INSTANTIATE_TEST_SUITE_P(InstantiationName, TestBackends, ::testing::ValuesIn(getParameters()));
