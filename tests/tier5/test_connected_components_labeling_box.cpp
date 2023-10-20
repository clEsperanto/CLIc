#include "cle.hpp"

#include <array>
#include <gtest/gtest.h>

class TestLabelingBox : public ::testing::TestWithParam<std::string>
{
protected:
  const std::array<uint32_t, 5 * 3 * 2> valid = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                  2, 2, 0, 1, 0, 0, 2, 0, 1, 1, 0, 2, 0, 0, 0 };
  const std::array<float, 5 * 3 * 2>    input = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                                                  1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0 };
  std::array<uint32_t, 5 * 3 * 2>       output;
};

TEST_P(TestLabelingBox, execute)
{
  std::string param = GetParam();
  cle::BackendManager::getInstance().setBackend(param);
  auto device = cle::BackendManager::getInstance().getBackend().getDevice("", "all");

  auto gpu_input = cle::Array::create(5, 3, 2, cle::dType::FLOAT, cle::mType::BUFFER, device);
  gpu_input->write(input.data());

  auto gpu_output = cle::tier5::connected_components_labeling_box_func(device, gpu_input, nullptr);

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

INSTANTIATE_TEST_CASE_P(InstantiationName, TestLabelingBox, ::testing::ValuesIn(getParameters()));