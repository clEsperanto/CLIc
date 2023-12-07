#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestVoronoiLabeling : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 6 * 6 * 1> valid = { 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2,
                                            3, 3, 3, 4, 4, 4, 3, 3, 3, 4, 4, 4, 3, 3, 3, 4, 4, 4 };

  std::array<uint32_t, 6 * 6 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };

  std::array<uint32_t, 6 * 6 * 1> output;
};

TEST_P(TestVoronoiLabeling, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(6, 6, 1,3 , cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier6::voronoi_labeling_func(device, gpu_input, nullptr);

  gpu_output->read(output.data());
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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestVoronoiLabeling, ::testing::ValuesIn(getParameters()));