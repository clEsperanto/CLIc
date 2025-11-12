#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestProximalNeighborCount : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 5 * 5 * 1> valid = { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 };

  const std::array<uint32_t, 5 * 5 * 1> input = { 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 5 };
  std::array<uint32_t, 5 * 5 * 1>       output;
};

TEST_P(TestProximalNeighborCount, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "gpu");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier6::proximal_neighbor_count_map_func(device, gpu_input, nullptr, 0, 3);

  gpu_output->readTo(output.data());
  for (int i = 0; i < output.size(); i++)
  {
    EXPECT_EQ(output[i], valid[i]);
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestProximalNeighborCount, ::testing::ValuesIn(getParameters()));
