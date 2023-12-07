#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestDetectLabelEdge : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint8_t, 5 * 5 * 1> output;
  std::array<uint8_t, 5 * 5 * 1> input = { 1, 1, 2, 2, 2, 1, 1, 2, 2, 2, 3, 3, 3, 2, 2, 3, 3, 3, 2, 0, 3, 3, 3, 0, 0 };
  std::array<uint8_t, 5 * 5 * 1> valid = { 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0 };
};

TEST_P(TestDetectLabelEdge, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(5, 5, 1,3 , cle::dType::UINT8, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier1::detect_label_edges_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestDetectLabelEdge, ::testing::ValuesIn(getParameters()));