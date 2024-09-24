#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestReduceLabelToCentroids : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 6 * 6 * 1> input = {
    0, 0, 0, 1, 1, 1, 0, 2, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 3, 3, 3, 4, 4, 4, 3, 3, 3, 4, 4, 4, 3, 3, 3, 4, 4, 4,
  };
  const std::array<uint32_t, 6 * 6 * 1> valid = {
    0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0,
  };
  std::array<uint32_t, 6 * 6 * 1> output;
};

TEST_P(TestReduceLabelToCentroids, executeBox)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 1, 3, cle::dType::LABEL, cle::mType::BUFFER, device);
  gpu_input->writeFrom(input.data());

  auto gpu_output = cle::tier5::reduce_labels_to_centroids_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestReduceLabelToCentroids, ::testing::ValuesIn(getParameters()));
