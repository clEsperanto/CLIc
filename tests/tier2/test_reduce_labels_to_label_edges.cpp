#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestReduceLabelsToLabelEdges : public ::testing::TestWithParam<std::string>
{
protected:
  std::array<uint32_t, 8 * 8 * 1> output;

  std::array<uint32_t, 8 * 8 * 1> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 0, 1, 0,
                                            1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 3, 3, 3, 4, 4, 4, 0, 0, 3, 0, 3,
                                            4, 0, 4, 0, 0, 3, 3, 3, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  std::array<uint32_t, 8 * 8 * 1> input = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 2, 0, 1, 1,
                                            1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 3, 3, 3, 4, 4, 4, 0, 0, 3, 3, 3,
                                            4, 4, 4, 0, 0, 3, 3, 3, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
};

TEST_P(TestReduceLabelsToLabelEdges, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");
  device->setWaitToFinish(true);

  auto gpu_input = cle::Array::create(8, 8, 1, 2, cle::dType::UINT32, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier2::reduce_labels_to_label_edges_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_SUITE_P(InstantiationName, TestReduceLabelsToLabelEdges, ::testing::ValuesIn(getParameters()));
