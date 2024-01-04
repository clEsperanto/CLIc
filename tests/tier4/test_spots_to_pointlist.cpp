
#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestSpotToPointList : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<float, 5 * 5 * 1>    input = { 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 };
  std::array<uint32_t, 4 * 2 * 1> valid = { 1, 3, 2, 4, 1, 1, 3, 4 };
  std::array<uint32_t, 4 * 2 * 1> output;
};

TEST_P(TestSpotToPointList, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier4::spots_to_pointlist_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestSpotToPointList, ::testing::ValuesIn(getParameters()));